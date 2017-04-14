#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sdobserver.h>

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
	void on_pushButton_close_clicked();

	void on_pushButton_hello_clicked();

	void on_pushButton_world_clicked();

	void on_pushButton_clicked();

	void on_pushButton_calc_clicked();


	void slot_sdMounted();
	void slot_sdUnmounted();

	void slot_sdMounted2();
	void slot_sdUnmounted2();

	void slot_sdMounted3();
	void slot_sdUnmounted3();


private:
	Ui::MainWindow *ui;

	QPolygonF m_circleData;


	SdObserver m_thread1;
	SdObserver m_thread2;
	SdObserver m_thread3;
	SdObserver m_thread4;
	SdObserver m_thread5;
};

#endif // MAINWINDOW_H
