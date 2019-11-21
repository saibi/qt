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

	QGeoServiceProvider * serviceProvider = new QGeoServiceProvider("osm");
	QGeoRoutingManager* routingManager = serviceProvider->routingManager();

	connect(routingManager, &QGeoRoutingManager::finished, this, &MainWindow::routeCalculated);
	connect(routingManager, &QGeoRoutingManager::error, this, &MainWindow::routeError);


	QGeoRouteRequest request(QGeoCoordinate(40.675895, -73.9562151), QGeoCoordinate(40.6833154, -73.987715));
	routingManager->calculateRoute(request);
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

void MainWindow::routeCalculated(QGeoRouteReply *reply)
{
	qDebug() << "Route Calculated";

	if ( reply->routes().size() != 0)
	{
		QGeoRoute route = reply->routes().at(0);
		qDebug() << route.path();
	}
	reply->deleteLater();
}


void MainWindow::routeError(QGeoRouteReply *reply, QGeoRouteReply::Error error, const QString &errorString)
{
	qDebug() << "Route Error" << errorString;
	reply->deleteLater();
}
