#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "chartitem.h"
#include "baritem.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	qmlRegisterType<ChartItem>("MyShape", 1, 0, "Chart");
	qmlRegisterType<BarItem>("MyShape", 1, 0, "Bar");

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/chart.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
