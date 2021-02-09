#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQuickItem>

#include "myclass.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QGuiApplication app(argc, argv);

	QQuickView view(QUrl("qrc:/main.qml"));
	QObject *item = view.rootObject();

	MyClass myClass;

	QObject::connect(item, SIGNAL(qmlSignal(QVariant)), &myClass, SLOT(cppSlot(QVariant)));

	view.show();

	return app.exec();
}
