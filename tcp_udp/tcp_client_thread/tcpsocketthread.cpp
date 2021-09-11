#include "tcpsocketthread.h"

#include <QDateTime>
#include <QElapsedTimer>


TcpSocketThread::TcpSocketThread(int socketDescriptor, QObject *parent) :
	QThread(parent), m_socketDescriptor(socketDescriptor), m_recvMutex(), m_sendMutex()
{
	m_connectMode = false;
	m_stopFlag = false;
}

TcpSocketThread::TcpSocketThread(const QHostAddress &address, quint16 port, QObject *parent) :
	QThread(parent), m_address(address), m_port(port), m_recvMutex(), m_sendMutex()
{
	m_connectMode = true;
	m_stopFlag = false;
}

void TcpSocketThread::slot_disconnected()
{
	qDebug("[TcpSocketThread::slot_disconnected]");

	if ( !m_stopFlag )
	{
		qDebug("DBG break; tcpsocket thread");

		m_stopFlag = true;
	}
}

void TcpSocketThread::slot_socketError(QAbstractSocket::SocketError socketError)
{
	if ( socketError != QAbstractSocket::SocketTimeoutError)
		qDebug("[TcpSocketThread::slot_socketError] %d", socketError);
}

void TcpSocketThread::slot_stateChanged(QAbstractSocket::SocketState socketState)
{
	qDebug("[TcpSocketThread::slot_stateChanged] %d", socketState);
}

void TcpSocketThread::run()
{
	QTcpSocket tcpSocket;

	if ( m_connectMode )
	{
		tcpSocket.connectToHost(m_address, m_port);

		if ( !tcpSocket.waitForConnected() )
		{
			qDebug() << "[TcpSocketThread::run] connectToHost error:" << tcpSocket.error();
			emit signalError(tcpSocket.error());
			return;
		}

		tcpSocket.write("Hello World\n", 12);
	}
	else
	{
		if ( !tcpSocket.setSocketDescriptor(m_socketDescriptor) )
		{
			qDebug() << "[TcpSocketThread::run] setSocketDescriptor error:" << tcpSocket.error();
			emit signalError(tcpSocket.error());
			return;
		}
	}

	connect(&tcpSocket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
	connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_socketError(QAbstractSocket::SocketError)));
	connect(&tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));


	qDebug("[TcpSocketThread::run] start loop");

	while (! m_stopFlag)
	{

		if ( tcpSocket.waitForReadyRead(5) )
		{
			QByteArray buf = tcpSocket.readAll();

			qDebug() << "[TcpSocketThread::run] recv" << buf.size() << "bytes :" << buf;
		}
	}

	qDebug("[TcpSocketThread::run] end loop");

	tcpSocket.close();

	if ( tcpSocket.state() == QAbstractSocket::UnconnectedState || tcpSocket.waitForDisconnected() )
		qDebug("[TcpSocketThread::run] disconnected");
	else
		qDebug() << "[TcpSocketThread::run] waitForDisconnected error:" << tcpSocket.error();

}


#if 0


void TcpSocketThread::run()
{
	QTcpSocket tcpSocket;

	if ( !tcpSocket.setSocketDescriptor(m_socketDescriptor) )
	{
		qDebug() << "DBG run() setSocketDescriptor" << tcpSocket.error();
		emit signalError(tcpSocket.error());
		return;
	}

#if QT_VERSION >= 0x050800
	connect(&tcpSocket, &QAbstractSocket::disconnected, this, [=]() {
		qDebug("DBG TcpSocketThread::run, disconnected");

		if ( !m_stopFlag )
		{
			qDebug("DBG break; tcpsocket thread");

			m_stopFlag = true;
		}
	});

	connect(&tcpSocket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), [=](QAbstractSocket::SocketError socketError) {
		if ( socketError != QAbstractSocket::SocketTimeoutError)
			qDebug("DBG socket error %d", socketError);
	});

	connect(&tcpSocket, &QAbstractSocket::stateChanged, [=](QAbstractSocket::SocketState socketState) {
		qDebug("DBG socket state %d", socketState);
	});
#else
	connect(&tcpSocket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
	connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_socketError(QAbstractSocket::SocketError)));
	connect(&tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));
#endif

	qDebug() << "DBG tcpsocketthread start";

	while (! m_stopFlag)
	{
		if ( m_recvBuf.size() > TcpPacket3::HEADER_SIZE )
		{
			QByteArray header = m_recvBuf.left(TcpPacket3::HEADER_SIZE);

			TcpPacket3 packet;
			int dataSize = packet.buildFromRawHeader(header);

			if ( dataSize > 0 )
			{
				if ( m_recvBuf.size() >= (dataSize + TcpPacket3::HEADER_SIZE) )
				{
					m_recvBuf.remove(0, TcpPacket3::HEADER_SIZE);
					packet.buildFromRawData(m_recvBuf.left(dataSize));
					m_recvBuf.remove(0, dataSize);
				}
				else
				{
					// need data
				}
			}
			else if ( dataSize == 0 )
			{
				m_recvBuf.remove(0, TcpPacket3::HEADER_SIZE);
			}
			else
			{
				int fsIdx = TcpPacket3::containsTcpPacket3Prefix(header);
				if ( fsIdx > 0 )
				{
					m_recvBuf.remove(0, fsIdx);
				}
			}

			if ( packet.isValid() )
			{
				QMutexLocker locker(&m_recvMutex);

				m_recvQ.enqueue(packet);

				qDebug() << "DBG packet received :" << packet.verifyChecksum() << packet;
			}
		}

		if ( tcpSocket.waitForReadyRead(5) )
		{
			QByteArray buf = tcpSocket.readAll();
			m_recvBuf += buf;
			qDebug("DBG %d bytes received. recvBuf %d", buf.size(), m_recvBuf.size());
		}

		{
			QMutexLocker locker(&m_sendMutex);

			if ( !m_sendQ.isEmpty() )
			{
				TcpPacket3 packet = m_sendQ.dequeue();

				int ret = sendPacketData(&tcpSocket, packet);
				qDebug("DBG send %d bytes", ret);
				if ( ret == 0 )
				{
					qDebug() << packet;
				}
			}
		}
	}

	qDebug() << "DBG tcpsocketthread end";

	tcpSocket.close();
	if ( tcpSocket.state() == QAbstractSocket::UnconnectedState || tcpSocket.waitForDisconnected() )
		qDebug() << "DBG disconnected";
	else
		qDebug() << "DBG waitForDisconnected() " << tcpSocket.error();
}


QHostAddress TcpSocketThread::clientAddress() const
{
	QTcpSocket tcpSocket;

	tcpSocket.setSocketDescriptor(m_socketDescriptor);
	return tcpSocket.peerAddress();
}

void TcpSocketThread::terminate()
{
	qDebug() << "Terminate tcpsocketthread";

	m_stopFlag = true;
}


void TcpSocketThread::sendPacket(const TcpPacket3 &packet)
{
	QMutexLocker locker(&m_sendMutex);

	if ( !packet.isValid() )
	{
		qDebug("DBG cannot send invalid packet");
		return;
	}
	m_sendQ.enqueue(packet);
}

bool TcpSocketThread::recvPacket(TcpPacket3 &packet)
{
	QMutexLocker locker(&m_recvMutex);

	if ( m_recvQ.isEmpty() )
		return false;

	packet = m_recvQ.dequeue();
	return true;
}


int TcpSocketThread::sendPacketData(QTcpSocket * psocket, const TcpPacket3 & packet)
{
	int ret = 0;
	const char *pdata = packet.rawByteArray().data();
	int remainSize = packet.rawByteArray().size();
	int sentSize = 0;

	while ( remainSize > 0 )
	{
		ret = psocket->write((const char *)&pdata[sentSize], (qint64)remainSize);
		if ( ret < 0 )
		{
			qDebug() << "DBG write error :" << psocket->errorString();
			break;
		}
		remainSize -= ret;
		sentSize += ret;
	}
	return sentSize;
}
#endif
