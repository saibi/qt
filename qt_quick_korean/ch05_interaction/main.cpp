#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickView>
#include <QQuickItem>
#include <QQmlProperty>

#include <QDebug>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QGuiApplication app(argc, argv);

	QQuickView view;
	view.setSource(QUrl("qrc:/main.qml"));
	view.show();

	QQuickItem *item = view.rootObject();
	item->setWidth(100);

	QObject * object = qobject_cast<QQuickItem*>(item);
	object->setProperty("height", 100);

	QQmlProperty::write(object, "someNumber", 18);

	qDebug() << "someNumber " << QQmlProperty::read(object, "someNumber").toInt();

	QObject *rect = object->findChild<QObject *>("rect");
	if ( rect )
		rect->setProperty("color", "blue");

	QVariant msg = "Hello from c++";
	QVariant retVal;
	QMetaObject::invokeMethod(object, "myFunction", Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, msg) );

	qDebug() << "QML function returned: " << retVal.toString();

	return app.exec();
}
