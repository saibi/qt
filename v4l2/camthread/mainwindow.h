#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();



private slots:

	void slotCamStream(char *camData, unsigned int offset);

	void on_pushButton_camStart_clicked();

protected:
	// YUYV to RGB test
	void conv1(unsigned char * dst, unsigned char * src, int width, int height);
	void conv2(unsigned char * dst, unsigned char * src, int width, int height);
	void conv3(QImage *pimg, unsigned char * src, int width, int height);
	void conv4(unsigned char * dst, unsigned char * src, int width, int height);

private:
	Ui::MainWindow *ui;

	unsigned char * pRGBData;

	int m_camWidth;
	int m_camHeight;
	bool m_camStartFlag;

	QPoint m_fbCamPos;
	int m_camSize;


	QVector<QRgb> m_yuv_rgb_conv_color_tab;
	QVector<QRgb> m_gray_colorize_color_tab;

	QImage m_image_indexed8;
	QImage m_image;
};

#endif // MAINWINDOW_H
