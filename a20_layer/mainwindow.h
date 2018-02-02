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
	void on_pushButton_green_clicked();

	void on_pushButton_img_clicked();


	void slotCamStream(char *camData, unsigned int offset);

	void on_pushButton_camStart_clicked();

	void on_pushButton_fbx_clicked();

	void on_pushButton_fby_clicked();

	void on_pushButton_qtDraw_clicked();

	void on_pushButton_fbDraw_clicked();

	void on_pushButton_disp_clicked();

	void on_pushButton_size_clicked();

	void on_pushButton_fill_clicked();

	void on_pushButton_colorkey_clicked();

	void on_pushButton_up_clicked();

	void on_pushButton_down_clicked();

	void on_pushButton_left_clicked();

	void on_pushButton_right_clicked();

	void on_pushButton_center_clicked();

	void on_pushButton_dispX_clicked();

	void on_pushButton_dispY_clicked();

	void on_pushButton_moveDisp_clicked();

	void on_pushButton_camSize_clicked();

	void on_pushButton_dispWidth_clicked();

	void on_pushButton_dispHeight_clicked();

	void on_pushButton_toggle_clicked();

	void on_pushButton_copy01_clicked();

	void on_pushButton_copy10_clicked();


	void on_pushButton_fb1Draw_clicked();

	void on_pushButton_fbLayer_clicked();

	void on_pushButton_fbLayerCk_clicked();

	void on_pushButton_fbLayerMv_clicked();

	void on_pushButton_neon_clicked();

	void on_pushButton_skip_clicked();

	void on_pushButton_gainUp_clicked();

	void on_pushButton_gainDn_clicked();

	void on_pushButton_expUp_clicked();

	void on_pushButton_expDn_clicked();

	void on_pushButton_mod_clicked();

	void on_pushButton_vflip_clicked();

	void on_pushButton_test1_clicked();

protected:
	bool inputIntValue(const QString & title, int min, int max, int & value);

private:
	Ui::MainWindow *ui;

	unsigned char * pRGBData;

	int m_camWidth;
	int m_camHeight;
	bool m_camStartFlag;

	QPoint m_fbCamPos;

	bool m_fbStream;
	bool m_qtStream;
	bool m_dispStream;
	bool m_fb1Stream;

	int m_fillSize;

	QPoint m_dispPos;
	QPoint m_dispSize;

	int m_camSize;


	QVector<QRgb> m_yuv_rgb_conv_color_tab;
	QVector<QRgb> m_gray_colorize_color_tab;

	QImage m_image_indexed8;

	bool m_neon;
	int m_skip;

	QImage m_image_cam, m_image_cam_gray;

	bool m_modifyCamBuf;
};

#endif // MAINWINDOW_H
