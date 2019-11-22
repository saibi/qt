#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connected = false;
	recording = false;

	camera = new QCamera();

	qDebug() << "Number of cameras found:" << QCameraInfo::availableCameras().count();

	QList <QCameraInfo> cameras = QCameraInfo::availableCameras();

	foreach( const QCameraInfo & cameraInfo, cameras)
	{
		qDebug() << "Camera info:" << cameraInfo.deviceName() << cameraInfo.description() << cameraInfo.position();

		ui->comboBox_device->addItem(cameraInfo.description());
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
	if ( !connected )
	{
		connectCamera();
	}
	else
	{
		camera->stop();
		viewfinder->deleteLater();
		ui->pushButton_connect->setText("Connect");
		connected = false;
	}
}

void MainWindow::connectCamera()
{
	QList <QCameraInfo> cameras = QCameraInfo::availableCameras();

	foreach( const QCameraInfo & cameraInfo, cameras)
	{
		qDebug() << cameraInfo.description() << ui->comboBox_device->currentText();

		if ( cameraInfo.description() == ui->comboBox_device->currentText() )
		{
			camera = new QCamera(cameraInfo);
			viewfinder = new QCameraViewfinder(this);
			camera->setViewfinder(viewfinder);
			ui->webcamLayout->layout()->addWidget(viewfinder);

			connect(camera, SIGNAL(error(QCamera::Error)), this, SLOT(cameraError(QCamera::Error)));

			connected = true;
			ui->pushButton_connect->setText("Disconnect");

			camera->start();

			return;
		}
	}
}

void MainWindow::cameraError(QCamera::Error error)
{
	qDebug() << "Camera error:" << error;

	connected = false;
	camera->stop();

	ui->pushButton_connect->setText("Connect");
}

void MainWindow::on_pushButton_capture_clicked()
{
	if ( connected )
	{
		imageCapture = new QCameraImageCapture(camera);
		camera->setCaptureMode(QCamera::CaptureStillImage);
		camera->searchAndLock();
		imageCapture->capture(qApp->applicationDirPath());
		camera->unlock();

		qDebug() << "capture saved in " << qApp->applicationDirPath();
	}
}

void MainWindow::on_pushButton_record_clicked()
{
	if ( connected )
	{
		if (!recording)
		{
			recorder = new QMediaRecorder(camera);
			connect(recorder, SIGNAL(error(QMediaRecorder::Error)), this, SLOT(recordError(QMediaRecorder::Error)));

			camera->setCaptureMode(QCamera::CaptureVideo);
			recorder->setOutputLocation(QUrl(qApp->applicationDirPath()));


			recorder->record();
			recording = true;

			qDebug() << "recording start : " << qApp->applicationDirPath();
		}
		else
		{
			recorder->stop();
			recording = false;
			qDebug() << "recording stop";
		}
	}
}


void MainWindow::recordError(QMediaRecorder::Error error)
{
	qDebug() << error;
}
