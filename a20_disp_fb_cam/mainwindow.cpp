#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framebuffer.h"
#include "keypaddlg.h"
#include "camthread.h"
#include "neon.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	pRGBData = new unsigned char [1280*1024*3];
	Q_CHECK_PTR(pRGBData);

	m_camStartFlag = false;
	m_camWidth = 0;
	m_camHeight = 0;
	m_qtStream = false;
	m_fbStream = false;

	connect(&CamThread::instance(), SIGNAL(signalCamStream(char*)), this, SLOT(slotCamStream(char*)));
}

MainWindow::~MainWindow()
{
	delete ui;

	delete [] pRGBData;
}

void MainWindow::on_pushButton_fb_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	FrameBuffer::instance().test(m_fbCamPos.x(), m_fbCamPos.y());

}

void MainWindow::on_pushButton_img_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	QPixmap img(":/global/images/run.jpg");

	FrameBuffer::instance().drawImg( m_fbCamPos.x(), m_fbCamPos.y(), img.toImage() );
}



void MainWindow::slotCamStream(char *camData)
{
	if ( m_camStartFlag )
	{
#ifdef __arm_A20__
		unsigned char *in = (unsigned char *)camData;

		nv21_to_rgb(pRGBData, in, in + m_camWidth * m_camHeight, m_camWidth, m_camHeight);

		QImage img((unsigned char *)pRGBData, m_camWidth, m_camHeight, QImage::Format_RGB888);

		if ( m_fbStream )
			FrameBuffer::instance().drawCam(m_fbCamPos.x(), m_fbCamPos.y(), img.convertToFormat(QImage::Format_RGB32).bits(), m_camWidth, m_camHeight);

		if ( m_qtStream )
			ui->label_cam->setPixmap(QPixmap::fromImage(img));
#endif
	}
}

void MainWindow::on_pushButton_camStart_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	CamThread::instance().startCam();

	m_camWidth = CamThread::instance().getCamWidth();
	m_camHeight = CamThread::instance().getCamHeight();

	m_camStartFlag = true;
}

void MainWindow::on_pushButton_camStop_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	m_camStartFlag = false;
	CamThread::instance().stopCam();
}

void MainWindow::on_pushButton_fbx_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = m_fbCamPos.x();

	if ( inputIntValue("framebuffer cam x pos", 0, 1024 - 1, val) )
	{
		m_fbCamPos.setX(val);

		ui->pushButton_fbx->setText(QString("fbx %1").arg(val));
	}
}

void MainWindow::on_pushButton_fby_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = m_fbCamPos.y();

	if ( inputIntValue("framebuffer cam y pos", 0, 768 - 1, val) )
	{
		m_fbCamPos.setY(val);

		ui->pushButton_fby->setText(QString("fby %1").arg(val));
	}
}

bool MainWindow::inputIntValue(const QString & title, int min, int max, int &value)
{
	KeypadDlg dlg(this);

	dlg.setWindowTitle(title);
	dlg.setKeypadMode(KeypadDlg::KM_INT);
	dlg.setIntMinMax(min, max);
	dlg.setIntStep(max - min < 100 ? 1 : 10);
	dlg.setIntValue(value);

	if ( QDialog::Accepted == dlg.exec() )
	{
		value = dlg.getIntValue();
		return true;
	}
	return false;
}

void MainWindow::on_pushButton_qtDraw_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_qtDraw->isChecked() )
		m_qtStream = true;
	else
		m_qtStream = false;
}


void MainWindow::on_pushButton_fbDraw_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_fbDraw->isChecked() )
		m_fbStream = true;
	else
		m_fbStream = false;

}
