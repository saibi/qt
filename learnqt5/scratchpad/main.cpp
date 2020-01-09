#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mocking.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	qDebug() << "Running the production code...";

	Client client1;
	client1.save();
	Client client2;
	client2.save();

	return app.exec();
}
