#include "imageviewer.h"
#include "ui_imageviewer.h"
#include "led.h"
#include "neon.h"

#include <QKeyEvent>
#include <QDebug>

ImageViewer::ImageViewer(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ImageViewer)
{
	ui->setupUi(this);

	m_stopFlag = false;

	pRGBData = new unsigned char[1280 * 1024 * 3]; // alloc max buffer
	Q_CHECK_PTR(pRGBData);

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

void ImageViewer::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Q)
	{
		qDebug() << "You Press q key ";
		m_stopFlag = true;
		disconnect(&myThread, SIGNAL(updateThread(int)), this, SLOT(updateGUI(int)));
		myThread.stop();
		close();
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
