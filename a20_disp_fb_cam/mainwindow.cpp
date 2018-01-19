#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framebuffer.h"
#include "keypaddlg.h"
#include "camthread.h"
#include "neon.h"
#include "disp.h"

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
	m_dispStream = false;
	m_fillSize = 100;
	m_camSize = CamThread::CAM_SIZE_480;
	connect(&CamThread::instance(), SIGNAL(signalCamStream(char*, unsigned int)), this, SLOT(slotCamStream(char*, unsigned int)));
}

MainWindow::~MainWindow()
{
	delete ui;

	delete [] pRGBData;
}

void MainWindow::on_pushButton_fb_clicked()
{
	qDebug("[%s] box test", Q_FUNC_INFO);

	FrameBuffer::instance().test(m_fbCamPos.x(), m_fbCamPos.y());

	ui->label_cam->clear();
	ui->label_cam->setStyleSheet("QLabel { background-color : #00ff00; }");
}

void MainWindow::on_pushButton_img_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	QPixmap img(":/global/images/run.jpg");

	FrameBuffer::instance().drawImg( m_fbCamPos.x(), m_fbCamPos.y(), img.toImage() );
}

void MainWindow::on_pushButton_size_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( inputIntValue("fill size", 1, 500, m_fillSize) )
	{
		ui->pushButton_size->setText(QString("%1").arg(m_fillSize));
	}
}

void MainWindow::on_pushButton_fill_clicked()
{
	static int idx = 0;
	unsigned int cTable[] = { 0, 0xffffffff, 0xff, 0xff00, 0xff0000, 0xff000000 };

	FrameBuffer::instance().fillBox(m_fbCamPos.x(), m_fbCamPos.y(), m_fillSize, m_fillSize, cTable[idx] );

	QString msg;
	msg.sprintf("fill 0x%x", cTable[idx]);
	ui->statusBar->showMessage(msg);

	qDebug("[%s] %s", Q_FUNC_INFO, qPrintable(msg));

	idx = (idx + 1) % 6;
}

void MainWindow::slotCamStream(char *camData, unsigned int offset)
{
#ifdef __arm_A20__
	if ( m_camStartFlag )
	{
		unsigned char *in = (unsigned char *)camData;

		nv21_to_rgb(pRGBData, in, in + m_camWidth * m_camHeight, m_camWidth, m_camHeight);

		QImage img((unsigned char *)pRGBData, m_camWidth, m_camHeight, QImage::Format_RGB888);

		if ( m_fbStream )
			FrameBuffer::instance().drawCam(m_fbCamPos.x(), m_fbCamPos.y(), img.convertToFormat(QImage::Format_RGB32).bits(), m_camWidth, m_camHeight);

		if ( m_qtStream )
			ui->label_cam->setPixmap(QPixmap::fromImage(img));

		if ( m_dispStream )
		{
			unsigned int addr = offset;
			Disp::instance().set_addr(m_camWidth, m_camHeight, &addr);
		}
	}
#else

	Q_UNUSED(camData);
	Q_UNUSED(offset);

#endif

}

void MainWindow::on_pushButton_camStart_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_camStart->isChecked() )
	{
		CamThread::instance().startCam(m_camSize);

		m_camWidth = CamThread::instance().getCamWidth();
		m_camHeight = CamThread::instance().getCamHeight();

		m_camStartFlag = true;
		ui->statusBar->showMessage("cam start");
		ui->pushButton_camStart->setText("Started");
	}
	else
	{
		m_camStartFlag = false;
		CamThread::instance().stopCam();
		ui->statusBar->showMessage("cam stop");
		ui->pushButton_camStart->setText("Stopped");
	}
}

void MainWindow::on_pushButton_fbx_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = m_fbCamPos.x();

	if ( inputIntValue("framebuffer x pos", 0, 1024 - 1, val) )
	{
		m_fbCamPos.setX(val);

		ui->pushButton_fbx->setText(QString("fbx %1").arg(val));
	}
}

void MainWindow::on_pushButton_fby_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = m_fbCamPos.y();

	if ( inputIntValue("framebuffer y pos", 0, 768 - 1, val) )
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

	if ( ui->pushButton_qtDraw->isChecked() )
		m_qtStream = true;
	else

		m_qtStream = false;

	ui->statusBar->showMessage(QString("qt stream %1").arg(m_qtStream));
	qDebug("[%s] qt stream = %d", Q_FUNC_INFO, m_qtStream);

}


void MainWindow::on_pushButton_fbDraw_clicked()
{
	if ( ui->pushButton_fbDraw->isChecked() )
		m_fbStream = true;
	else
		m_fbStream = false;

	ui->statusBar->showMessage(QString("fb stream = %1").arg(m_fbStream));
	qDebug("[%s] fb stream = %d", Q_FUNC_INFO, m_fbStream);
}

void MainWindow::on_pushButton_disp_clicked()
{
	if ( ui->pushButton_disp->isChecked() )
	{
		if ( m_camStartFlag )
		{
			Disp::instance().init(m_dispPos.x(), m_dispPos.y(), m_camWidth, m_camHeight);
			Disp::instance().set_para(m_camWidth, m_camHeight);
			Disp::instance().start();
			m_dispStream = true;
		}
		else
		{
			qDebug("[%s] start cam first", Q_FUNC_INFO);
		}
	}
	else
	{
		m_dispStream = false;
		Disp::instance().stop();
		Disp::instance().quit();
	}

	ui->statusBar->showMessage(QString("disp stream = %1").arg(m_dispStream));

	qDebug("[%s] disp stream = %d", Q_FUNC_INFO, m_dispStream);
}

void MainWindow::on_pushButton_colorkey_clicked()
{
	if ( ui->pushButton_colorkey->isChecked() )
	{
		bool ret = Disp::instance().enableColorKey( FrameBuffer::instance().getLayerId(), 0xff00 );

		qDebug("[%s] enable colorkey %d", Q_FUNC_INFO, ret);
	}
	else
	{
		bool ret = Disp::instance().disableColorKey();
		qDebug("[%s] disable colorkey %d", Q_FUNC_INFO, ret);
	}
}

void MainWindow::on_pushButton_up_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().adjRow(true);
}

void MainWindow::on_pushButton_down_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().adjRow(false);
}

void MainWindow::on_pushButton_left_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().adjCol(true);
}

void MainWindow::on_pushButton_right_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().adjCol(false);
}

void MainWindow::on_pushButton_center_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().resetRowCol();
}

void MainWindow::on_pushButton_dispX_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = m_dispPos.x();

	if ( inputIntValue("disp x pos", 0, 1024 - 1, val) )
	{
		m_dispPos.setX(val);

		ui->pushButton_dispX->setText(QString("x %1").arg(val));
	}
}

void MainWindow::on_pushButton_dispY_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = m_dispPos.y();

	if ( inputIntValue("disp y pos", 0, 768 - 1, val) )
	{
		m_dispPos.setY(val);

		ui->pushButton_dispY->setText(QString("y %1").arg(val));
	}
}

void MainWindow::on_pushButton_moveDisp_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	Disp::instance().move(m_dispPos.x(), m_dispPos.y());
	if ( ui->pushButton_colorkey->isChecked() )
	{
		Disp::instance().disableColorKey();
		Disp::instance().enableColorKey( FrameBuffer::instance().getLayerId(), 0xff00 );
	}
}

void MainWindow::on_pushButton_camSize_clicked()
{

	if ( m_camSize == CamThread::CAM_SIZE_480 )
	{
		m_camSize = CamThread::CAM_SIZE_640;
		ui->pushButton_camSize->setText("640");
	}
	else if ( m_camSize == CamThread::CAM_SIZE_640)
	{
		m_camSize = CamThread::CAM_SIZE_960;
		ui->pushButton_camSize->setText("960");
	}
	else if ( m_camSize == CamThread::CAM_SIZE_960)
	{
		m_camSize = CamThread::CAM_SIZE_1280;
		ui->pushButton_camSize->setText("1280");
	}
	else
	{
		m_camSize = CamThread::CAM_SIZE_480;
		ui->pushButton_camSize->setText("480");
	}

	qDebug("[%s] m_camSize = %d", Q_FUNC_INFO, m_camSize);
}
