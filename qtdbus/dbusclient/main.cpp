#include <QCoreApplication>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QString serviceName("org.saibi.QtDBus.HelloService");
	QDBusInterface serviceInterface(serviceName, "/");
	QDBusReply<QString> response = serviceInterface.call(
				"sayHello", "saibi");

	qDebug().noquote() << response;

	return a.exec();
}
