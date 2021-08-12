#include "udpserver.h"
#include <QRegExp>


#define EW_MYIP "ew myip"
#define EW_LIST "ew list"
#define EW_IP "ew ip"


UdpServer::UdpServer(QObject *parent) : QObject(parent)
{
}

bool UdpServer::startServer(unsigned short port, unsigned int expire)
{
	if ( port == 0 )
		port = DEFAULT_PORT;

	if ( expire == 0 )
		expire = DEFAULT_EXPIRE_TIME;

	m_expire = expire;

	qDebug() << "start server - port:" << port << ", expire(min):" << m_expire;

	m_udpSocket = new QUdpSocket(this);

	if ( m_udpSocket && m_udpSocket->bind(QHostAddress::Any, port) )
	{
		connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpServer::slotReadPendingDatagrams );
		startTimer(CHECK_PERIOD * 1000);

		connect(m_udpSocket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), [=](QAbstractSocket::SocketError socketError)
		{
			qDebug() << "error occured :" << socketError;
			emit signalError();
		}
		);
	}
	else
	{
		qDebug() << "server error:" << m_udpSocket->errorString();
		return false;
	}
	return true;
}

void UdpServer::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);

	QDateTime now = QDateTime::currentDateTime();
	unsigned int expireSec = m_expire * 60;

	QMap<QString, QDateTime>::iterator i = m_map.begin();
	while (i != m_map.end())
	{
		if ( i.value().secsTo(now) >= expireSec )
		{
			qDebug() << "ip expired:" << i.key() << i.value().toString("HH:mm:ss");

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

	if ( data.startsWith(EW_MYIP) )
	{
		QString ip(data);
		ip = ip.replace(EW_MYIP, "").trimmed();
		if ( ip.contains(QRegExp("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$")) )
		{
			m_map.insert(ip, QDateTime::currentDateTime());

			qDebug() << "device ip added :" << ip;
		}
	}
	else if ( data.startsWith(EW_LIST) )
	{
		QMap<QString, QDateTime>::const_iterator i = m_map.constBegin();
		while (i != m_map.constEnd())
		{
			QByteArray answer = EW_IP " ";

			answer += i.key().toLocal8Bit();

			m_udpSocket->writeDatagram(answer, datagram.senderAddress(), datagram.senderPort() );
			++i;
		}
	}
}
