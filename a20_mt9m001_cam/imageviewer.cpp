#include "imageviewer.h"
#include "ui_imageviewer.h"
#include "led.h"
#include "neon.h"
#include "clickablelabel.h"
#include "keypaddlg.h"

#include <QKeyEvent>
#include <QDebug>

ImageViewer::ImageViewer(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ImageViewer)
{
	ui->setupUi(this);

	m_ctrlIdx = 0;
	m_ids.append(V4L2_CID_AUDIO_TREBLE);
	m_ids.append(V4L2_CID_AUDIO_BASS);
	m_ids.append(V4L2_CID_EXPOSURE);
	m_ids.append(V4L2_CID_GAIN);

	m_stopFlag = false;

	pRGBData = new unsigned char[1280 * 1024 * 3]; // alloc max buffer
	Q_CHECK_PTR(pRGBData);

	connect(ui->label, SIGNAL(clicked(QPoint)), this, SLOT(slot_clicked(QPoint)));
	connect(&myThread, SIGNAL(updateThread(int)), this, SLOT(updateGUI(int)));
	myThread.start();

}

ImageViewer::~ImageViewer()
{
	if ( pRGBData )
	{
		delete [] pRGBData;
		pRGBData = 0;
	}

	delete ui;
}

void ImageViewer::handleClose()
{
	m_stopFlag = true;
	disconnect(&myThread, SIGNAL(updateThread(int)), this, SLOT(updateGUI(int)));
	myThread.stop();
	close();
}

void ImageViewer::slot_clicked(const QPoint &pos)
{
	qDebug("(%d,%d)", pos.x(), pos.y());

	if ( pos.x() > 900 && pos.y() < 50 )
	{
		handleClose();
		return;
	}

	if ( pos.y() < 100 )
	{
		// up
		upControl_rowStart();
	}
	else if ( pos.y() > 650 )
	{
		// down
		downControl_rowStart();
	}
	else
	{
		if ( pos.x() > 500 )
		{
			if ( pos.x() > 900 )
			{
				// right
				downControl_colStart();
			}
			else
			{
				// gain
				if ( pos.y() < 380 )
				{
					upControl_gain();
				}
				else
				{
					downControl_gain();
				}
			}
		}
		else
		{
			if ( pos.x() < 100 )
			{
				// left
				upControl_colStart();
			}
			else
			{
				// exposure
				if ( pos.y() < 350 )
				{
					upControl_exp();
				}
				else
				{
					downControl_exp();
				}

			}
		}
	}
}

void ImageViewer::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Q)
	{
		qDebug() << "You Press q key ";
		handleClose();
		return;
	}
	else if(event->key() == Qt::Key_A){
		qDebug() << "A key => Up Gain";
		upControl_gain();
	}
	else if(event->key() == Qt::Key_D){
		qDebug() << "D key => Down Gain ";
		downControl_gain();
	}
	else if(event->key() == Qt::Key_S){
		qDebug() << "S key => Up Exposure";
		upControl_exp();
	}
	else if(event->key() == Qt::Key_F){
		qDebug() << "F key => Down Exposure";
		downControl_exp();
	}
	else if(event->key() == Qt::Key_K){
		qDebug() << "Up key => Up Row Start";
		upControl_rowStart();
	}
	else if(event->key() == Qt::Key_J){
		qDebug() << "Down key => Down Row Start";
		downControl_rowStart();
	}
	else if(event->key() == Qt::Key_H){
		qDebug() << "Left key => Left Col Start";
		upControl_colStart();
	}
	else if(event->key() == Qt::Key_L){
		qDebug() << "Right key => Right Col Start";
		downControl_colStart();
	}
	else
	{
		qDebug() << event->nativeScanCode();
	}
}

void ImageViewer::updateGUI(int index)
{
#ifndef __arm_A20__
	Q_UNUSED(index);
#endif

	if ( m_stopFlag || pRGBData == 0 )
		return;

	g_led_on();

#ifdef __arm_A20__
	unsigned char *in = (unsigned char *)buffers[index].start;

	///NEON NEON NEON NEON NEON NEON FORMAT NV21
	nv21_to_rgb(pRGBData, in, in + cam_width * cam_height, cam_width, cam_height);

	ui->label->setPixmap(QPixmap::fromImage(QImage((unsigned char *)pRGBData, cam_width, cam_height, QImage::Format_RGB888)));
#endif

	g_led_off();
}

void ImageViewer::on_pushButton_exit_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handleClose();
}

void ImageViewer::handleCtrl(const QString &title, int id, int min, int max)
{
	KeypadDlg dlg(this);

	dlg.setKeypadMode(KeypadDlg::KM_INT);
	dlg.setIntMinMax(min, max);
	dlg.setIntStep(1);
	dlg.setIntValue(0);

	dlg.setWindowTitle(title);

	if ( QDialog::Accepted == dlg.exec() )
	{
		if ( vid_s_ctrl(id, dlg.getIntValue()) != 0)
			qDebug("[%s] s_ctrl err - %s 0x%x %d ", Q_FUNC_INFO, qPrintable(title), id, dlg.getIntValue());
	}
}

void ImageViewer::on_pushButton_vflip_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handleCtrl("V4L2_CID_VFLIP", V4L2_CID_VFLIP, 0, 1);
}


void ImageViewer::on_pushButton_gain_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handleCtrl("V4L2_CID_GAIN", V4L2_CID_GAIN, 0, 127);
}

void ImageViewer::on_pushButton_exposure_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handleCtrl("V4L2_CID_EXPOSURE", V4L2_CID_EXPOSURE, 0, 1023);

}
