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

		if ( !m_sendQ.isEmpty() )
		{
			QMutexLocker locker(&m_sendMutex);

			TcpPacket3 packet = m_sendQ.dequeue();

			int ret = tcpSocket.write(packet.rawByteArray());
			qDebug("DBG send packet %d bytes", ret);
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
