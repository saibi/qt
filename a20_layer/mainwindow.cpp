#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "keypaddlg.h"
#include "camthread.h"
#include "neon.h"
#include "a20disp.h"

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
	m_fb1Stream = false;
	m_neon = true;
	m_skip = 0;
	connect(&CamThread::instance(), SIGNAL(signalCamStream(char*, unsigned int)), this, SLOT(slotCamStream(char*, unsigned int)));


	int i, hue, saturation;
	for(i = 0 ; i < 256 ; i++)
	{
		m_yuv_rgb_conv_color_tab.append(qRgb(i,i,i));

		hue = 255 - i;
		saturation = 255;
		m_gray_colorize_color_tab.append(QColor::fromHsv(hue, saturation, 255).rgb());
	}


	A20Disp::instance().init();
}

MainWindow::~MainWindow()
{
	A20Disp::instance().quit();

	delete ui;

	delete [] pRGBData;
}

void MainWindow::on_pushButton_green_clicked()
{
	qDebug("[%s] box test", Q_FUNC_INFO);

	ui->label_cam->clear();
	ui->label_cam->setStyleSheet("QLabel { background-color : #00ff00; }");
}

void MainWindow::on_pushButton_img_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	QImage img(":/global/images/run.jpg");

	A20Disp::instance().drawImg(m_fbCamPos.x(), m_fbCamPos.y(), img);
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

	A20Disp::instance().fillBox(m_fbCamPos.x(), m_fbCamPos.y(), m_fillSize, m_fillSize, cTable[idx] );

	QString msg;
	msg.sprintf("fill 0x%x", cTable[idx]);
	ui->statusBar->showMessage(msg);

	qDebug("[%s] %s", Q_FUNC_INFO, qPrintable(msg));

	idx = (idx + 1) % 6;
}

void MainWindow::slotCamStream(char *camData, unsigned int offset)
{
#ifdef __arm_A20__
	static int count = 0;

	if ( m_camStartFlag )
	{
		if ( count < m_skip )
		{
			++count;
			return;
		}
		else
			count = 0;

		if ( m_dispStream )
			A20Disp::instance().setCamBufAddr(offset);


		if ( m_neon )
		{
			// use neon
			unsigned char *in = (unsigned char *)camData;

			nv21_to_rgb(pRGBData, in, in + m_camWidth * m_camHeight, m_camWidth, m_camHeight);

			QImage img((unsigned char *)pRGBData, m_camWidth, m_camHeight, QImage::Format_RGB888);

			if ( m_fbStream || m_fb1Stream )
			{
				const QImage &img32 = img.convertToFormat(QImage::Format_RGB32);
				if ( m_fbStream )
					A20Disp::instance().drawCam(m_fbCamPos.x(), m_fbCamPos.y(), (unsigned char *)img32.bits(), m_camWidth, m_camHeight, A20Disp::FB_MAIN);
				else
					A20Disp::instance().drawCam(0, 0, (unsigned char *)img32.bits(), m_camWidth, m_camHeight, A20Disp::FB_BACK);
			}

			if ( m_qtStream )
				ui->label_cam->setPixmap(QPixmap::fromImage(img));
		}
		else
		{
			// Use QImage Indexed8
			::memcpy(m_image_indexed8.bits(), camData, m_image_indexed8.byteCount());

			if ( m_fbStream || m_fb1Stream )
			{
				const QImage & img32 = m_image_indexed8.convertToFormat(QImage::Format_RGB32);

				if ( m_fbStream )
					A20Disp::instance().drawCam(m_fbCamPos.x(), m_fbCamPos.y(), (unsigned char *)img32.bits(), m_camWidth, m_camHeight, A20Disp::FB_MAIN);
				else
					A20Disp::instance().drawCam(0, 0, (unsigned char *)img32.bits(), m_camWidth, m_camHeight, A20Disp::FB_BACK);
			}

			if ( m_qtStream )
				ui->label_cam->setPixmap(QPixmap::fromImage(m_image_indexed8));
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
		m_dispSize.setX(m_camWidth);
		m_dispSize.setY(m_camHeight);

		m_image_indexed8 = QImage(m_camWidth, m_camHeight, QImage::Format_Indexed8);
		m_image_indexed8.setColorTable(m_yuv_rgb_conv_color_tab);

		m_camStartFlag = true;
		ui->statusBar->showMessage("cam start");
		ui->pushButton_camStart->setText("Started");

		ui->pushButton_dispWidth->setText(QString::number(m_camWidth));
		ui->pushButton_dispHeight->setText(QString::number(m_camHeight));
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
			A20Disp::instance().addCamBufLayer(m_camWidth, m_camHeight, m_dispPos.x(), m_dispPos.y(), m_dispSize.x(), m_dispSize.y());
			A20Disp::instance().showLayer(A20Disp::LAYER_CAM_BUF);
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

		A20Disp::instance().hideLayer(A20Disp::LAYER_CAM_BUF);
		A20Disp::instance().removeLayer(A20Disp::LAYER_CAM_BUF);
	}

	ui->statusBar->showMessage(QString("disp stream = %1").arg(m_dispStream));

	qDebug("[%s] disp stream = %d", Q_FUNC_INFO, m_dispStream);
}

void MainWindow::on_pushButton_colorkey_clicked()
{
	if ( ui->pushButton_colorkey->isChecked() )
	{
		bool ret = A20Disp::instance().enableColorKey(A20Disp::LAYER_CAM_BUF, 0xff00 );
		qDebug("[%s] enable colorkey %d", Q_FUNC_INFO, ret);
	}
	else
	{
		bool ret = A20Disp::instance().disableColorKey(A20Disp::LAYER_CAM_BUF);
		qDebug("[%s] disable colorkey %d", Q_FUNC_INFO, ret);
	}
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

	A20Disp::instance().moveLayer(A20Disp::LAYER_CAM_BUF, m_dispPos.x(), m_dispPos.y(), m_dispSize.x(), m_dispSize.y());
	if ( ui->pushButton_colorkey->isChecked() )
	{
		A20Disp::instance().disableColorKey(A20Disp::LAYER_CAM_BUF);
		A20Disp::instance().enableColorKey(A20Disp::LAYER_CAM_BUF, 0xff00);
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

void MainWindow::on_pushButton_dispWidth_clicked()
{
	int val = m_dispSize.x();

	if ( inputIntValue("disp width", 1, 1024, val) )
	{
		m_dispSize.setX(val);
		ui->pushButton_dispWidth->setText(QString::number(val));
		ui->statusBar->showMessage(QString("disp width = %1").arg(val));
	}
	qDebug("[%s] disp width = %d", Q_FUNC_INFO, val);
}

void MainWindow::on_pushButton_dispHeight_clicked()
{
	int val = m_dispSize.y();

	if ( inputIntValue("disp height", 1, 768, val) )
	{
		m_dispSize.setY(val);
		ui->pushButton_dispHeight->setText(QString::number(val));
		ui->statusBar->showMessage(QString("disp height = %1").arg(val));
	}
	qDebug("[%s] disp height = %d", Q_FUNC_INFO, val);
}

void MainWindow::on_pushButton_toggle_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	if ( ui->pushButton_toggle->isChecked() )
		A20Disp::instance().toggleFbArea(A20Disp::FB_BACK);
	else
		A20Disp::instance().toggleFbArea(A20Disp::FB_MAIN);
}

void MainWindow::on_pushButton_copy01_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	A20Disp::instance().copyFb(A20Disp::FB_MAIN);
}

void MainWindow::on_pushButton_copy10_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	A20Disp::instance().copyFb(A20Disp::FB_BACK);
}


void MainWindow::on_pushButton_fb1Draw_clicked()
{
	if ( ui->pushButton_fb1Draw->isChecked() )
		m_fb1Stream = true;
	else
		m_fb1Stream = false;

	ui->statusBar->showMessage(QString("fb1 stream = %1").arg(m_fb1Stream));
	qDebug("[%s] fb1 stream = %d", Q_FUNC_INFO, m_fb1Stream);
}

void MainWindow::on_pushButton_fbLayer_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if (ui->pushButton_fbLayer->isChecked() )
	{
		A20Disp::instance().addFbBackBufLayer(m_camWidth, m_camHeight, m_dispPos.x(), m_dispPos.y(), m_dispSize.x(), m_dispSize.y());
		A20Disp::instance().showLayer(A20Disp::LAYER_FB_BACK_BUF);
	}
	else
	{
		A20Disp::instance().removeLayer(A20Disp::LAYER_FB_BACK_BUF);
	}
}

void MainWindow::on_pushButton_fbLayerCk_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_fbLayer->isChecked() )
	{
		if ( ui->pushButton_fbLayerCk->isChecked() )
			A20Disp::instance().enableColorKey(A20Disp::LAYER_FB_BACK_BUF, 0xff00);
		else
			A20Disp::instance().disableColorKey(A20Disp::LAYER_FB_BACK_BUF);
	}
}

void MainWindow::on_pushButton_fbLayerMv_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);


	A20Disp::instance().moveLayer(A20Disp::LAYER_FB_BACK_BUF, m_dispPos.x(), m_dispPos.y(), m_dispSize.x(), m_dispSize.y());
	if ( ui->pushButton_fbLayerCk->isChecked() )
	{
		A20Disp::instance().disableColorKey(A20Disp::LAYER_FB_BACK_BUF);
		A20Disp::instance().enableColorKey(A20Disp::LAYER_FB_BACK_BUF, 0xff00);
	}
}

void MainWindow::on_pushButton_neon_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_neon->isChecked() )
	{
		m_neon = false;
		ui->pushButton_neon->setText("indexed8");
	}
	else
	{
		m_neon = true;
		ui->pushButton_neon->setText("neon");
	}
}

void MainWindow::on_pushButton_skip_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	m_skip = (m_skip + 1) % 4;
	ui->pushButton_skip->setText(QString("skip %1").arg(m_skip));

	ui->statusBar->showMessage(QString("skip %1").arg(m_skip));
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
	CamThread::instance().resetRowCol();

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

