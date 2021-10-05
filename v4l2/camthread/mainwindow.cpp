#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camthread.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_camStartFlag = false;
	m_camWidth = 0;
	m_camHeight = 0;
	m_camSize = CamThread::CAM_SIZE_640;

	connect(&CamThread::instance(), SIGNAL(signalCamStream(char*, unsigned int)), this, SLOT(slotCamStream(char*, unsigned int)));


	int i, hue, saturation;
	for(i = 0 ; i < 256 ; i++)
	{
		m_yuv_rgb_conv_color_tab.append(qRgb(i,i,i));

		hue = 255 - i;
		saturation = 255;
		m_gray_colorize_color_tab.append(QColor::fromHsv(hue, saturation, 255).rgb());
	}

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slotCamStream(char *camData, unsigned int offset)
{
	Q_UNUSED(offset);

	if ( m_camStartFlag )
	{
#if 0
		// Use QImage Indexed8
		::memcpy(m_image_indexed8.bits(), camData, m_image_indexed8.byteCount());
		ui->label_cam->setPixmap(QPixmap::fromImage(m_image_indexed8));
#endif

		// YUYV to RGB test

		//conv3(&m_image, (unsigned char *)camData, m_camWidth, m_camHeight);
		conv4((unsigned char*)m_image.bits(), (unsigned char *)camData, m_camWidth, m_camHeight);
		ui->label_cam->setPixmap(QPixmap::fromImage(m_image));
	}
}


void MainWindow::conv1(unsigned char * dst, unsigned char * src, int width, int height)
{
	int i;
	int Y1,Y2,U,V;

	for( i=0; i < width * height; i = i+2 )
	{
		Y1=src[i*2+1];
		Y2=src[i*2+3];
		U=src[i*2];
		V=src[i*2+2];

		dst[i*3+2]=(unsigned char)((float)Y1+(1.4075f*(float)(V-128)));  // Red
		dst[i*3+1]=(unsigned char)((float)Y1+(0.3455f*(float)(U-128)-(0.7169f*(float)(V-128))));  // Green
		dst[i*3]=(unsigned char)((float)Y1+(1.7790f*(float)(U-128)));    // Blue
		dst[i*3+5]=(unsigned char)((float)Y2+(1.4075f*(float)(V-128)));  // Red
		dst[i*3+4]=(unsigned char)((float)Y2+(0.3455f*(float)(U-128)-(0.7169f*(float)(V-128))));  // Green
		dst[i*3+3]=(unsigned char)((float)Y2+(1.7790f*(float)(U-128)));  // Blue
	}
}

void MainWindow::conv2(unsigned char *dst, unsigned char *src, int width, int height)
{

	for(int i = 0, j=0; i < width * height * 3; i+=6, j+=4)
	{

		unsigned char u = src[j];
		unsigned char y = src[j+1];
		unsigned char v = src[j+2];

		dst[i+2] = 1.0*y + 8 + 1.402*(v-128);               // r
		dst[i+1] = 1.0*y - 0.34413*(u-128) - 0.71414*(v-128);   // g
		dst[i] = 1.0*y + 1.772*(u-128) + 0;                            // b

		y = src[j+3];
		dst[i+5] = 1.0*y + 8 + 1.402*(v-128);               // r
		dst[i+4] = 1.0*y - 0.34413*(u-128) - 0.71414*(v-128);   // g
		dst[i+3] = 1.0*y + 1.772*(u-128) + 0;
	}
}

void MainWindow::conv3(QImage *pimg, unsigned char *src, int width, int height)
{
	int step = width * 2;
	int size = width * height;

	for (int i = 0; i< height; i++)
	{
	  for (int j=0; j< width; j++)
	  {
		float Y = src[i*step + j];
		float U = src[ (int)(size + (i/2)*(step/2)  + j/2) ];
		float V = src[ (int)(size*1.25 + (i/2)*(step/2) + j/2)];

		float R = Y + 1.402 * (V - 128);
		float G = Y - 0.344 * (U - 128) - 0.714 * (V - 128);
		float B = Y + 1.772 * (U - 128);


		if (R < 0){ R = 0; } if (G < 0){ G = 0; } if (B < 0){ B = 0; }
		if (R > 255 ){ R = 255; } if (G > 255) { G = 255; } if (B > 255) { B = 255; }

		pimg->setPixel( j, i, qRgb(R, G, B));
	  }
	}
}

void MainWindow::conv4(unsigned char *dst, unsigned char *src, int width, int height)
{
	int i;
	float y1, y2, u, v;
	int size = width * height * 2;

	for (i=0; i < size; i+=4) {

		y1 = src[i];
		u = src[i+1];
		y2 = src[i+2];
		v = src[i+3];

		dst[0] = (y1 + 1.371*(u - 128.0));
		dst[1] = (y1 - 0.698*(u - 128.0) - 0.336*(v - 128.0));
		dst[2] = (y1 + 1.732*(v - 128.0));
		dst[3] = (y2 + 1.371*(v - 128.0));
		dst[4] = (y2 - 0.698*(v - 128.0) - 0.336*(u - 128.0));
		dst[5] = (y2 + 1.732*(u - 128.0));

		dst += 6;
	}
}

void MainWindow::on_pushButton_camStart_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	if ( ui->pushButton_camStart->isChecked() )
	{
		CamThread::instance().startCam(m_camSize);

		m_camWidth = CamThread::instance().getCamWidth();
		m_camHeight = CamThread::instance().getCamHeight();

		m_image_indexed8 = QImage(m_camWidth, m_camHeight, QImage::Format_Indexed8);
		m_image_indexed8.setColorTable(m_yuv_rgb_conv_color_tab);
		qDebug("m_image_indexed8 byteCount %d", m_image_indexed8.byteCount());

		m_image = QImage(m_camWidth, m_camHeight, QImage::Format_RGB888);
		qDebug("m_image byteCount %d", m_image.byteCount());

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
