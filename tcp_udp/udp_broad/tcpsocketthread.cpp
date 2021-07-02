#include "tcpsocketthread.h"

#include <QDateTime>
#include <QElapsedTimer>

#include "tcppacket.h"

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
	QElapsedTimer stopwatch;

	stopwatch.start();


	int transfer = m_transfer;
	if ( transfer < 10 )
		transfer = 10;

	int repeat = m_repeat;
	if ( repeat < 1 )
		repeat = 1;

	while (! m_stopFlag)
	{
		for ( int i = 0 ; i < repeat ; ++i )
		{
			QString date = QDateTime::currentDateTime().toString("yyyy.MM.ddThh:mm:ss");

			QByteArray data;

			for ( int i = 0 ; i < transfer / 10; ++i)
			{
				data.append(QString::asprintf("#%8d;", i).toLocal8Bit());
			}

			TcpPacket packet;

			if ( i % 2 == 0 )
			{
				packet.set("currentDateTime=" + date);
			}
			else
			{
				data.prepend(date.toLocal8Bit());
				packet.set("file name_" + QString::number(i), data);
			}

			int send_bytes = 0;
			int ret = tcpSocket.write(packet.header());
			qDebug() << "write" << ret <<"bytes";
			send_bytes += ret;

			if ( packet.type() == TcpPacket::TCP_PACKET_HAVE_DATA )
			{
				ret = tcpSocket.write(packet.data());
				qDebug() << "write" << ret <<"bytes";
				send_bytes += ret;
			}

			qDebug() << "#" << i+1 << ": Send" << send_bytes << "bytes :" << date;

#if 0
			int send_bytes = 0;


			for ( int i = 0 ; i < transfer / 10; ++i)
				data.append("1234567890");

			QString date = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss;");

			qDebug() << "#" << i+1 << "============================================================";

			int ret = tcpSocket.write(date.toLocal8Bit());
			qDebug() << "write" << ret <<"bytes";
			send_bytes += ret;

			ret = tcpSocket.write(data);
			qDebug() << "write" << ret <<"bytes";
			send_bytes += ret;

			ret = tcpSocket.write(QString::asprintf("#%8d#", i+1).toLocal8Bit());
			qDebug() << "write" << ret <<"bytes";
			send_bytes += ret;

			qDebug() << "Send" << send_bytes << "bytes :" << date;

#endif

			int recv_bytes = 0;
			QByteArray recv_data;
			recv_data.clear();
			while ( recv_bytes < TcpPacket::HEADER_SIZE )
			{
				if ( tcpSocket.waitForReadyRead(1000) )
				{
					QByteArray data = tcpSocket.readAll();
					qDebug() << "read header" << data.size() << "bytes";
					recv_bytes += data.size();
					recv_data += data;
				}
			}
			TcpPacket recvPacket;

			int data_size = recvPacket.fromByteArray(recv_data);
			if ( data_size < 0 )
			{
				qDebug() << "Recv unknown packet" << recv_bytes << "bytes [" << recv_data.left(40) << "~" << recv_data.right(4) << "]";
			}
			else
			{
				qDebug() << "Recv header :" << recvPacket.header();

				if ( data_size > 0 )
				{
					recv_bytes = 0;
					recv_data.clear();
					while ( recv_bytes < data_size )
					{
						if ( tcpSocket.waitForReadyRead(1000) )
						{
							QByteArray data = tcpSocket.readAll();
							qDebug() << "read data" << data.size() << "bytes";
							recv_bytes += data.size();
							recv_data += data;
						}
					}
					qDebug() << "Recv data" << recv_bytes << "bytes [" << recv_data.left(40) << "~" << recv_data.right(4) << "]";
				}
			}
		}
		qDebug() << "DBG break";
		m_stopFlag = true;
	}

	tcpSocket.close();
	if ( tcpSocket.state() == QAbstractSocket::UnconnectedState || tcpSocket.waitForDisconnected() )
		qDebug() << "DBG disconnected";
	else
		qDebug() << "DBG waitForDisconnected() " << tcpSocket.error();


	qDebug() << "DBG tcpsocketthread finished " << stopwatch.elapsed() << "ms elapsed";
}


QHostAddress TcpSocketThread::clientAddress() const
{
	QTcpSocket tcpSocket;

	tcpSocket.setSocketDescriptor(m_socketDescriptor);
	return tcpSocket.peerAddress();
}
