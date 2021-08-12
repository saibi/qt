#include <QCoreApplication>
#include <QCommandLineParser>

#include "udpserver.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QCoreApplication::setApplicationName("discovery-server");
	QCoreApplication::setApplicationVersion("0.1");

	QCommandLineParser parser;

	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption portOption(QStringList() << "p" << "port",
			QCoreApplication::translate("main", "set receive port"),
			QCoreApplication::translate("main", "port number"));

	QCommandLineOption expireOption(QStringList() << "e" << "expire",
			QCoreApplication::translate("main", "set expiration time (min)"),
			QCoreApplication::translate("main", "minutes"));

	parser.addOption(portOption);
	parser.addOption(expireOption);

	parser.process(a);

	QString portStr = parser.value(portOption);
	QString expireStr = parser.value(expireOption);

	UdpServer s;

	if ( !s.startServer(portStr.toInt(), expireStr.toInt()) )
		return 1;

	QObject::connect(&s, &UdpServer::signalError, &a, &QCoreApplication::quit);
	return a.exec();
}
