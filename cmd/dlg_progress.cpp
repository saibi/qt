#include "dlg_progress.h"
#include "ui_dlg_progress.h"

#include <QDateTime>

Dlg_Progress::Dlg_Progress(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dlg_Progress)
{
	ui->setupUi(this);
	m_maxSec = 60;
	m_started = false;
}

Dlg_Progress::~Dlg_Progress()
{
	delete ui;
}

void Dlg_Progress::showEvent(QShowEvent *e)
{
	ui->progressBar->setValue(0);
	m_timerId = startTimer(0.2 * 1000); // 0.2 seconds

	QTime curTime = QDateTime::currentDateTime().time();
	qDebug("start %02d%02d%02d", curTime.hour(), curTime.minute(), curTime.second());

	m_started = true;
}
void Dlg_Progress::hideEvent(QHideEvent *e)
{
	if ( m_timerId > 0 )
	{
		killTimer(m_timerId);
		m_started = false;
	}
}

void Dlg_Progress::timerEvent(QTimerEvent *e)
{
	int val = ui->progressBar->value();
	val ++;
	QTime curTime = QDateTime::currentDateTime().time();
	qDebug("timer %02d%02d%02d", curTime.hour(), curTime.minute(), curTime.second());

	if ( val > ui->progressBar->maximum() )
	{
		killTimer(e->timerId());
		qDebug("end %02d%02d%02d", curTime.hour(), curTime.minute(), curTime.second());
		this->accept();
	}
	else
	{
		ui->progressBar->setValue(val);
	}
}

void Dlg_Progress::setTime(int seconds)
{
	if ( seconds > 0 )
	{
		m_maxSec = seconds;
	}
	ui->progressBar->setMaximum(m_maxSec * 5);
}

void Dlg_Progress::terminate()
{
	killTimer(m_timerId);

	QTime curTime = QDateTime::currentDateTime().time();
	int endTime = curTime.second() + 2;
	int curValue = ui->progressBar->value();

	do
	{
		curTime = QDateTime::currentDateTime().time();
		if ( curValue < ui->progressBar->maximum())
		{
			ui->progressBar->setValue(curValue);
			curValue++;
		}
		QApplication::processEvents();
	} while ( curTime.second() < endTime);
	if ( curValue < ui->progressBar->maximum())
	{
		ui->progressBar->setValue(ui->progressBar->maximum());
		QApplication::processEvents();
	}
	this->accept();
}

void Dlg_Progress::setPercent(float percent)
{
	ui->progressBar->setValue( ui->progressBar->maximum() * percent / 100.0);
	QApplication::processEvents();
}

bool Dlg_Progress::isStarted()
{
	return m_started;
}
