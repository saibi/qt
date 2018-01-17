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
	void on_pushButton_fb_clicked();

	void on_pushButton_img_clicked();


	void slotCamStream(char *camData, unsigned int offset);

	void on_pushButton_camStart_clicked();

	void on_pushButton_camStop_clicked();

	void on_pushButton_fbx_clicked();

	void on_pushButton_fby_clicked();

	void on_pushButton_qtDraw_clicked();

	void on_pushButton_fbDraw_clicked();

	void on_pushButton_disp_clicked();

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

};

#endif // MAINWINDOW_H
