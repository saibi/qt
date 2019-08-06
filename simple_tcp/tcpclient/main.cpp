#include <QCoreApplication>

#include <QTcpSocket>
#include <QObject>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QTcpSocket *tcpSocket = new QTcpSocket(0);
	tcpSocket->connectToHost("127.0.0.1", 5000);

	QObject::connect(tcpSocket, &QTcpSocket::connected, [tcpSocket] {
		qDebug() << "connected";
	});

	QObject::connect(tcpSocket, &QTcpSocket::readyRead, [tcpSocket] {
		qDebug() << QString::fromUtf8(tcpSocket->readAll());
	});

	QObject::connect(tcpSocket, &QTcpSocket::disconnected, [tcpSocket] {
		qDebug() << "disconnected";
	});

	return a.exec();
}
