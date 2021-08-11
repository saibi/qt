#include "udpserver.h"

UdpServer::UdpServer(unsigned short port, QObject *parent) : QObject(parent)
{
	m_udpSocket = new QUdpSocket(this);

	if ( port == 0 )
		port = DEFAULT_PORT;

	m_udpSocket->bind(QHostAddress::LocalHost, port);

	connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpServer::slotReadPendingDatagrams );

	startTimer(CHECK_PERIOD * 1000);
}

void UdpServer::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);

	QDateTime now = QDateTime::currentDateTime();

	QMap<QString, QDateTime>::iterator i = m_map.begin();
	while (i != m_map.end())
	{
		qDebug() << i.key() << ": " << i.value() << " " << i.value().secsTo(now) << ((i.value().secsTo(now) < (VALID_TIME * 1000)) ? "valid" : "expired");

		if ( i.value().secsTo(now) >= VALID_TIME )
		{
			qDebug() << "delete" << i.key();

			i = m_map.erase(i);
		}
		else
			++i;
	}
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

	if ( data.startsWith("ew myip") )
	{
		QString ip(data);
		ip = ip.replace("ew myip", "").trimmed();
		m_map.insert(ip, QDateTime::currentDateTime());

		QMap<QString, QDateTime>::const_iterator i = m_map.constBegin();
		while (i != m_map.constEnd()) {
			qDebug() << i.key() << ": " << i.value();
			++i;
		}
	}
}
