#include "udpserver.h"

UdpServer::UdpServer(unsigned short port, QObject *parent) : QObject(parent)
{
	m_udpSocket = new QUdpSocket(this);

	if ( port == 0 )
		port = DEFAULT_PORT;

	m_udpSocket->bind(QHostAddress::LocalHost, port);

	connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpServer::slotReadPendingDatagrams );

	qDebug() << m_udpSocket;
}


void UdpServer::slotReadPendingDatagrams()
{
	while (m_udpSocket->hasPendingDatagrams())
	{

		QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
		processDatagram(datagram);
	}

}

void UdpServer::processDatagram(QNetworkDatagram datagram)
{
	QByteArray data = datagram.data().trimmed();

	qDebug() << datagram.senderAddress() << datagram.senderPort();
	qDebug() << data;
}
