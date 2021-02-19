#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include "tcpconnection.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QGuiApplication app(argc, argv);

	qmlRegisterType<TcpConnection>("TCP", 1, 0, "TcpConnection");
	QQuickView view;

	view.setSource(QUrl("qrc:/main.qml"));
	view.show();

	return app.exec();
}
