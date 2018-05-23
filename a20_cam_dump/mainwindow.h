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

	void on_pushButton_camStart_clicked();

	void on_pushButton_up_clicked();

	void on_pushButton_down_clicked();

	void on_pushButton_left_clicked();

	void on_pushButton_right_clicked();

	void on_pushButton_center_clicked();

	void on_pushButton_gainUp_clicked();

	void on_pushButton_gainDn_clicked();

	void on_pushButton_expUp_clicked();

	void on_pushButton_expDn_clicked();

	void on_pushButton_vflip_clicked();

	void on_pushButton_capture_clicked();

	void on_pushButton_hflip_clicked();

	void on_pushButton_camSize_clicked();

	void on_pushButton_gainTest_clicked();

	void on_pushButton_gainVal_clicked();

	void on_pushButton_expVal_clicked();

	void on_pushButton_colVal_clicked();

	void on_pushButton_rowVal_clicked();

	void on_pushButton_videoStart_clicked();

	void on_pushButton_videoStop_clicked();

	void on_pushButton_exit_clicked();

protected:
	bool inputIntValue(const QString & title, int min, int max, int & value);

protected slots:
	void slotImageCapture(QImage *image);

	void slotGainTest();

private:
	Ui::MainWindow *ui;

	int m_camWidth;
	int m_camHeight;
	bool m_camStartFlag;
	int m_camSize;

	QVector<QRgb> m_yuv_rgb_conv_color_tab;
	QImage m_image;
	int m_captureRecv;
	enum CaptureRecvType
	{
		CAPTURE_SHOW = 0,
		CAPTURE_GAIN_TEST = 1,
	};

	int m_gainTestVal;
	QList <QImage> m_imgList;
};

#endif // MAINWINDOW_H
