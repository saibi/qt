#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <ellipseitem.h>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	qmlRegisterType<EllipseItem>("MyShape", 1, 0, "Ellipse");


	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/qquickpainteditem.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
