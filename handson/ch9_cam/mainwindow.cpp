#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connected = false;

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
