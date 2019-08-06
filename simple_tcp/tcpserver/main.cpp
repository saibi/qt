#include <QCoreApplication>

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QTcpServer *tcpServer = new QTcpServer(0);
	tcpServer->listen(QHostAddress::Any, 5000);

	QObject::connect(tcpServer, &QTcpServer::newConnection, [tcpServer] {
		QTcpSocket *tcpSocket = tcpServer->nextPendingConnection();
		QByteArray response = QString("Hello World from QTcpServer\n").toUtf8();
		tcpSocket->write(response);
		tcpSocket->disconnectFromHost();
		qDebug() << "Send response and close the socket";
	});

	return a.exec();
}
