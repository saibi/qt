#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QUrl>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();


	void connectCamera();

private slots:
	void on_pushButton_connect_clicked();

	void cameraError(QCamera::Error error);

	void on_pushButton_capture_clicked();

	void on_pushButton_record_clicked();

	void recordError(QMediaRecorder::Error error);

private:
	Ui::MainWindow *ui;

	QCamera * camera;
	QCameraViewfinder * viewfinder;
	QCameraImageCapture * imageCapture;
	QMediaRecorder * recorder;

	bool connected;
	bool recording;
};

#endif // MAINWINDOW_H
