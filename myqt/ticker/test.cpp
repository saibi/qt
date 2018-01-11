#include <QHBoxLayout>
#include <QMessageBox>
#include <QEvent>
#include <QApplication>
#include <QTime>
#include <QProgressDialog>
#include <QCloseEvent>

#include "test.h"

Test::Test(QWidget *parent) : QDialog(parent)
{
	threadA.setMessage("A");
	threadB.setMessage("B");

	threadAButton = new QPushButton(tr("Start A"));
	threadBButton = new QPushButton(tr("Start B"));
	quitButton = new QPushButton(tr("Quit"));
	quitButton->setDefault(true);
	
	connect(threadAButton, SIGNAL(clicked()), this, SLOT(startOrStopThreadA()));
	connect(threadBButton, SIGNAL(clicked()), this, SLOT(startOrStopThreadB()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

	QHBoxLayout *h_layout = new QHBoxLayout;
	h_layout->addWidget(threadAButton);
	h_layout->addWidget(threadBButton);
	h_layout->addWidget(quitButton);


	label = new QLabel(tr("test"));
	label->installEventFilter(this);


	ticker = new Ticker;
	ticker->setText("0123456789 ");
	ticker->installEventFilter(this);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	layout->addWidget(ticker);
	layout->addLayout(h_layout);
	setLayout(layout);

}

bool Test::eventFilter(QObject *target, QEvent *event)
{
	if ( event->type() == QEvent::MouseButtonPress) {
		if ( target == ticker) {
			qApp->beep();
			QMessageBox::information(this, tr("ticker"), tr("ticker\nprogress bar test\nwait a minute."));

			int start = (QTime::currentTime()).minute();
			int sec = (QTime::currentTime()).second();

			QProgressDialog progress(this);
			progress.setLabelText(tr("wait a minute"));
			progress.setRange(0, 59 - sec);
			progress.setModal(true);

			while(1) {
				progress.setValue( QTime::currentTime().second() - sec);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				if ( start >= 59 ) {
					if ( (QTime::currentTime()).minute() == 0 ) {
						break;
					}
				} else {
					if ( QTime::currentTime().minute() > start ) {
						break;
					}
				}
				if ( progress.wasCanceled()) {
					QMessageBox::information(this, tr("ticker"), tr("canceled by user."));
					break;
				}
			}
			QMessageBox::information(this, tr("ticker"), tr("test end."));
		} else if ( target == label ) {
			QMessageBox::information(this, tr("label"), tr("label label"));
		}
	}
	return QDialog::eventFilter(target, event);
}


void Test::startOrStopThreadA()
{
	if ( threadA.isRunning()) {
		threadA.stop();
		threadAButton->setText(tr("Start A"));
	} else {
		threadA.start();
		threadAButton->setText(tr("Stop A"));
	}
}

void Test::startOrStopThreadB()
{
	if ( threadB.isRunning()) {
		threadB.stop();
		threadBButton->setText(tr("Start B"));
	} else {
		threadB.start();
		threadBButton->setText(tr("Stop B"));
	}
}


void Test::closeEvent(QCloseEvent *event)
{
	threadA.stop();
	threadB.stop();
	threadA.wait();
	threadB.wait();
	event->accept();
}


