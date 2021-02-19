#include "tcpconnection.h"

#include <QTcpServer>
#include <QTcpSocket>

#include <QDebug>


TcpConnection::TcpConnection(QObject *parent) : QObject(parent), m_hostName("127.0.0.1")
{
}

void TcpConnection::sendData(const QString &data)
{
	m_tcpSocket->write( data.toUtf8() + "\n");
	qDebug() << "sendData " << data.toUtf8();
}


int TcpConnection::port() const
{
	return m_port;
}

void TcpConnection::setPort(int port)
{
	if ( m_port != port )
	{
		m_port = port;
		emit portChanged();
		qDebug() << "port " << port;
	}
}

QString TcpConnection::hostName() const
{
	return m_hostName;
}

void TcpConnection::setHostName(const QString &hostName)
{
	if ( m_hostName != hostName )
	{		
		m_hostName = hostName;
		emit hostNameChanged();
		qDebug() << "hostName " << hostName;

	}
}

TcpConnection::ConnectionType TcpConnection::connectionType() const
{
	return m_connectionType;
}

void TcpConnection::setConnectionType(ConnectionType connectionType)
{
	if ( m_connectionType != connectionType )
	{
		m_connectionType = connectionType;
		emit connectionTypeChanged();
		qDebug() << "ConnectionType " << connectionType;
	}
}


void TcpConnection::initialize()
{
	if ( m_connectionType == Server )
	{
		qDebug() << "Initialize Server";
		m_tcpServer = new QTcpServer;
		m_tcpServer->listen( QHostAddress(m_hostName), m_port);
		connect( m_tcpServer, SIGNAL(newConnection()), this, SLOT(slotConnection()));
	}
	else
	{
		qDebug() << "Initialize Client";

		m_tcpSocket = new QTcpSocket(this);
		m_tcpSocket->connectToHost(m_hostName, m_port);
		connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(receivedData()) );

	}
}

void TcpConnection::slotConnection()
{
	m_tcpSocket = m_tcpServer->nextPendingConnection();
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(receivedData()));
	qDebug() << "slotConnection";

}

void TcpConnection::receivedData()
{
	const QString txt = QString::fromUtf8(m_tcpSocket->readAll());
	emit dataReceived( txt );
	qDebug() << "receivedData " << txt;

}
