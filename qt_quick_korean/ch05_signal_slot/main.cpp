#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQuickItem>
#include <QDateTime>

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

	QVariantList list;

	list << 10 << QColor(Qt::green) << "bottles";

	QVariantMap map;

	map.insert("language", "QML");
	map.insert("released", QDate(2015, 9, 21));

	QMetaObject::invokeMethod(item, "readValues", Q_ARG(QVariant, QVariant::fromValue(list)), Q_ARG(QVariant, QVariant::fromValue(map)));

	QDateTime dateTime = QDateTime::currentDateTime();
	QVariant retValue;

	QMetaObject::invokeMethod(item, "readDate", Q_RETURN_ARG(QVariant, retValue), Q_ARG(QVariant, QVariant::fromValue(dateTime)) );

	qDebug() << "Value returned from readDAte():" << retValue.toDateTime();
	view.show();

	return app.exec();
}
