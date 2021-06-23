#include "tcpsocketthread.h"

#include <QDateTime>

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

	int count = 0;

	while (! m_stopFlag)
	{
		if ( tcpSocket.waitForReadyRead(2000) )
		{
			QByteArray data = tcpSocket.readAll();
			qDebug() << "Recv" << data.size() << "bytes =====\n" << data << "\n";


			++count;
			if ( count > 700 )
			{
				qDebug() << "DBG break";
				m_stopFlag = true;
			}
		}
		else
		{
			QByteArray data;

			for ( int i = 0 ; i < count; ++i)
			{
				data.append("1234567890");
			}

			QString date = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss;");
			int ret = tcpSocket.write(date.toLocal8Bit());
			qDebug() << "write" << ret <<" bytes";

			ret = tcpSocket.write(data);
			qDebug() << "write" << ret <<" bytes";

			qDebug() << "Send :" << date;
		}

	}
	tcpSocket.close();
	if ( tcpSocket.state() == QAbstractSocket::UnconnectedState || tcpSocket.waitForDisconnected() )
		qDebug() << "DBG disconnected";
	else
		qDebug() << "DBG waitForDisconnected() " << tcpSocket.error();


	qDebug() << "DBG tcpsocketthread finished";
}
