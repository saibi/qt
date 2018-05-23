#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "keypaddlg.h"

#include "a20camthread.h"
#include "a20disp.h"

#include <QMessageBox>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_camStartFlag = false;
	m_camWidth = 0;
	m_camHeight = 0;
	m_camSize = CamThread::SIZE_480s;

	for(int i = 0 ; i < 256 ; i++)
	{
		m_yuv_rgb_conv_color_tab.append(qRgb(i,i,i));
	}

	m_captureRecv = CAPTURE_SHOW;

	m_gainTestVal = -1;

	connect(&CamThread::instance(), SIGNAL(imageCapture(QImage*)), this, SLOT(slotImageCapture(QImage*)));

}


MainWindow::~MainWindow()
{
	CamThread::instance().closeCameraDevice();
	delete ui;
}

void MainWindow::on_pushButton_camStart_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_camStart->isChecked() )
	{
		CamThread::instance().setCompensationOn(false);
		CamThread::instance().setTopLeftOnFB( ui->label_cam->mapToGlobal(QPoint(0,0)));
		CamThread::instance().startCam(CamThread::ImageCamera, QPoint(0,0), m_camSize);

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


void MainWindow::on_pushButton_up_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().adjRow(true);

	ui->statusBar->showMessage(QString("row %1").arg(CamThread::instance().getRow()));
}

void MainWindow::on_pushButton_down_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().adjRow(false);

	ui->statusBar->showMessage(QString("row %1").arg(CamThread::instance().getRow()));
}

void MainWindow::on_pushButton_left_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().adjCol(true);

	ui->statusBar->showMessage(QString("col %1").arg(CamThread::instance().getCol()));
}

void MainWindow::on_pushButton_right_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().adjCol(false);

	ui->statusBar->showMessage(QString("col %1").arg(CamThread::instance().getCol()));
}

void MainWindow::on_pushButton_center_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	CamThread::instance().moveCam(0, 0);

	ui->statusBar->showMessage(QString("row %1 col %2").arg(CamThread::instance().getRow()).arg(CamThread::instance().getCol()));
}

void MainWindow::on_pushButton_gainUp_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	CamThread::instance().adjGain(true);

	ui->statusBar->showMessage(QString("gain %1").arg(CamThread::instance().getGain()));
}

void MainWindow::on_pushButton_gainDn_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	CamThread::instance().adjGain(false);

	ui->statusBar->showMessage(QString("gain %1").arg(CamThread::instance().getGain()));
}

void MainWindow::on_pushButton_expUp_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	CamThread::instance().adjExp(true);

	ui->statusBar->showMessage(QString("exposure %1").arg(CamThread::instance().getExp()));
}

void MainWindow::on_pushButton_expDn_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	CamThread::instance().adjExp(false);

	ui->statusBar->showMessage(QString("exposure %1").arg(CamThread::instance().getExp()));
}

void MainWindow::on_pushButton_vflip_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_vflip->isChecked() )
		CamThread::instance().vflip(false);
	else
		CamThread::instance().vflip(true);
}

void MainWindow::on_pushButton_hflip_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_hflip->isChecked() )
		CamThread::instance().hflip(false);
	else
		CamThread::instance().hflip(true);
}


void MainWindow::on_pushButton_capture_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( !CamThread::instance().isRunning() )
	{
		QMessageBox::information(this, "error", "start camera first", QMessageBox::Ok);
		return;
	}

	qDebug("capture indexed8 %d", QImage::Format_Indexed8);
	if ( m_image.width() != m_camWidth || m_image.height() != m_camHeight )
	{
		m_image = QImage(m_camWidth, m_camHeight, QImage::Format_Indexed8);
		m_image.setColorTable(m_yuv_rgb_conv_color_tab);
	}

	m_captureRecv = CAPTURE_SHOW;
	CamThread::instance().captureCam(&m_image);
}

void MainWindow::slotImageCapture(QImage *image)
{
	if ( m_captureRecv == CAPTURE_SHOW)
	{
		QLabel tmpLabel;

		tmpLabel.setMinimumSize(m_camWidth, m_camHeight);
		tmpLabel.setPixmap(QPixmap::fromImage(*image));
		tmpLabel.show();

		QMessageBox::information(this, "image captured", "press the ok button.", QMessageBox::Ok);
	}
	else if ( m_captureRecv == CAPTURE_GAIN_TEST )
	{
		qDebug("DBG cam %d, gain %d img captured.", m_camSize, CamThread::instance().getGain());
		m_imgList.append(*image);


		++m_gainTestVal;
		QTimer::singleShot(200, this, SLOT(slotGainTest()));
	}
}

void MainWindow::on_pushButton_camSize_clicked()
{
	if ( m_camSize == CamThread::SIZE_480s )
	{
		m_camSize = CamThread::SIZE_960;
		ui->pushButton_camSize->setText("960");
	}
	else
	{
		m_camSize = CamThread::SIZE_480s;
		ui->pushButton_camSize->setText("480");
	}
	qDebug("[%s] cam size %d", Q_FUNC_INFO, m_camSize);
}

void MainWindow::on_pushButton_gainTest_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);


	qDebug("DBG start gain test ");

	if ( !CamThread::instance().isRunning() )
	{
		QMessageBox::information(this, "error", "start camera first", QMessageBox::Ok);
		return;
	}

	m_imgList.clear();

	if ( m_image.width() != m_camWidth || m_image.height() != m_camHeight )
	{
		m_image = QImage(m_camWidth, m_camHeight, QImage::Format_Indexed8);
		m_image.setColorTable(m_yuv_rgb_conv_color_tab);
	}

	m_gainTestVal = 0;
	QTimer::singleShot(200, this, SLOT(slotGainTest()));
}

void MainWindow::slotGainTest()
{
	if ( m_gainTestVal < 0 )
		return;

	if ( m_gainTestVal > 63 )
	{
		qDebug("DBG gain test end. %d imgs captured", m_imgList.size());
		m_gainTestVal = -1;

		for (int i = 0 ; i < m_imgList.size(); ++i )
		{
			m_imgList.at(i).save( QString("/tmp/cam%1_gain%2.png").arg(m_camSize).arg(i, 2, 10, QChar('0')));
			if ( m_camSize == CamThread::SIZE_960 )
				m_imgList.at(i).scaled(480, 480).save(QString("/tmp/cam%1to480_gain%2.png").arg(m_camSize).arg(i, 2, 10, QChar('0')));

			qDebug("DBG img #%d saved", i);
		}
		return;
	}

	qDebug("DBG gain test - val %d", m_gainTestVal);
	if ( !CamThread::instance().setGain(m_gainTestVal) )
		qDebug("DBG setGain error");

	m_captureRecv = CAPTURE_GAIN_TEST;
	CamThread::instance().captureCam(&m_image);
}

void MainWindow::on_pushButton_gainVal_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = CamThread::instance().getGain();
	if ( inputIntValue("Gain", 0, 63, val) )
	{
		if (CamThread::instance().setGain(val) )
			ui->statusBar->showMessage(QString("gain %1").arg(CamThread::instance().getGain()));
		else
			qDebug("[%s] setGain error", Q_FUNC_INFO);
	}
}

void MainWindow::on_pushButton_expVal_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = CamThread::instance().getExp();
	if ( inputIntValue("Exposure", 0, 127, val) )
	{
		if (CamThread::instance().setExp(val) )
			ui->statusBar->showMessage(QString("exp %1").arg(CamThread::instance().getExp()));
		else
			qDebug("[%s] setExp error", Q_FUNC_INFO);
	}

}

void MainWindow::on_pushButton_rowVal_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = CamThread::instance().getRow();
	if ( inputIntValue("Row", 0, 511, val) )
	{
		if (CamThread::instance().setRow(val) )
			ui->statusBar->showMessage(QString("row %1").arg(CamThread::instance().getRow()));
		else
			qDebug("[%s] setRow error", Q_FUNC_INFO);
	}

}

void MainWindow::on_pushButton_colVal_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	int val = CamThread::instance().getCol();
	if ( inputIntValue("col", 0, 639, val) )
	{
		if (CamThread::instance().setCol(val) )
			ui->statusBar->showMessage(QString("row %1").arg(CamThread::instance().getCol()));
		else
			qDebug("[%s] setCol error", Q_FUNC_INFO);
	}
}

void MainWindow::on_pushButton_videoStart_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( A20Disp::instance().layerVideoStartStop(true) )
		ui->statusBar->showMessage("video start");
}

void MainWindow::on_pushButton_videoStop_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( A20Disp::instance().layerVideoStartStop(false) )
		ui->statusBar->showMessage("video stop");
}

void MainWindow::on_pushButton_exit_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	CamThread::instance().closeCameraDevice();
	close();
}
