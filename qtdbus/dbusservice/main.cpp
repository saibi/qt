#include <QCoreApplication>

#include <QtDBus/QDBusConnection>
#include "helloservice.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	HelloService helloService;

	QString serviceName("org.saibi.QtDBus.HelloService");

	QDBusConnection::sessionBus().registerService(serviceName);
	QDBusConnection::sessionBus().registerObject("/", &helloService, QDBusConnection::ExportAllSlots);

	return a.exec();
}
