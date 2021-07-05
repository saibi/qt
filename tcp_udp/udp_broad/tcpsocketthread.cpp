#include "tcpsocketthread.h"

#include <QDateTime>
#include <QElapsedTimer>

#include "tcppacket2.h"

TcpSocketThread::TcpSocketThread(int socketDescriptor, QObject *parent) :
	QThread(parent), m_socketDescriptor(socketDescriptor)
{
}

void TcpSocketThread::run()
{
	QTcpSocket tcpSocket;

	if ( !tcpSocket.setSocketDescriptor(m_socketDescriptor) )
	{
		   emit signalError(tcpSocket.error());
		   return;
	}

	qDebug() << "DBG tcpsocketthread start";

	while (! m_stopFlag)
	{
		if ( m_sendTest )
		{
			int transfer = m_transfer;
			if ( transfer < 10 )
				transfer = 10;

			int repeat = m_repeat;
			if ( repeat < 1 )
				repeat = 1;

			QByteArray data;

			for ( int i = 0 ; i < transfer / 10; ++i)
			{
				data.append(QString::asprintf("#%8d;", i).toLocal8Bit());
			}

			for ( int i = 0 ; i < repeat ; ++i )
			{
				TcpPacket2 packet;

				if ( (i % 3) == 2 )
				{
					packet.set(TcpPacket2::FLAG_NONE, TcpPacket2::CMD_NONE + i, data);
				}
				else
					packet.set(TcpPacket2::FLAG_NONE, TcpPacket2::CMD_NONE + i);

				int ret = tcpSocket.write(packet.rawData());
				qDebug() << "#" << i+1 << ": Send" << ret << "bytes.";

				if ( packet.rawData().size() != ret )
					qDebug("rawData().size() %d != %d", packet.rawData().size(), ret);
			}

			m_sendTest = false;
		}

		if ( tcpSocket.waitForReadyRead(500) )
		{
			QByteArray data = tcpSocket.readAll();
			qDebug() << "recv" << data.size() << "bytes";
		}
	}

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

void TcpSocketThread::sendTest()
{
	qDebug() << "DBG sendTest start";

	m_sendTest = true;

//	QElapsedTimer stopwatch;

//	stopwatch.start();

//	qDebug() << "DBG send " << stopwatch.elapsed() << "ms elapsed";

}
