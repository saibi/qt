#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQuickItem>
#include <QQuickView>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QGeoPositionInfoSource * source = QGeoPositionInfoSource::createDefaultSource(this);
	if ( source )
	{
		connect(source, &QGeoPositionInfoSource::positionUpdated, this, &MainWindow::positionUpdated);
		source->startUpdates();
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::positionUpdated(const QGeoPositionInfo &info)
{
	qDebug() << "Position updated:" << info;



	QObject * target = qobject_cast<QObject *>(ui->quickWidget->rootObject());

	QMetaObject::invokeMethod(target, "addMarker", Qt::AutoConnection, Q_ARG(QVariant, "HUVITZ"), Q_ARG(QVariant, info.coordinate().latitude()), Q_ARG(QVariant, info.coordinate().longitude()));

	qDebug() << info.coordinate().latitude() << info.coordinate().longitude();
}
