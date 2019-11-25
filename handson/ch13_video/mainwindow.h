#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QTime>
#include <QVideoWidget>

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
	void stateChanged(QMediaPlayer::State state);
	void positionChanged(qint64 position);

	void on_actionOpen_File_triggered();

	void on_pushButton_play_clicked();

	void on_pushButton_pause_clicked();

	void on_pushButton_stop_clicked();

	void on_horizontalSlider_vol_sliderMoved(int position);

	void on_horizontalSlider_pos_sliderMoved(int position);

private:
	Ui::MainWindow *ui;

	QMediaPlayer * player;
};

#endif // MAINWINDOW_H
