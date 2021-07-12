#include "tcpsocketthread.h"

#include <QDateTime>
#include <QElapsedTimer>


TcpSocketThread::TcpSocketThread(int socketDescriptor, QObject *parent) :
	QThread(parent), m_socketDescriptor(socketDescriptor), m_recvMutex(), m_sendMutex()
{
}

void TcpSocketThread::run()
{
	QTcpSocket tcpSocket;

	if ( !tcpSocket.setSocketDescriptor(m_socketDescriptor) )
	{
		qDebug() << "DBG run() setSocketDescriptor" << tcpSocket.error();
		emit signalError(tcpSocket.error());
		return;
	}

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
			qDebug("DBG write error");
			continue;
		}
		remainSize -= ret;
		sentSize += ret;
	}
	return sentSize;
}
