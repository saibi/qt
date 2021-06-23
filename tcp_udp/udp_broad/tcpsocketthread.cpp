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

	int count = 5;

	while (! m_stopFlag)
	{
		qDebug() << (tcpSocket.isValid() ? "valid" : "not valid");

		if ( tcpSocket.waitForReadyRead(1000) )
		{
			QByteArray data = tcpSocket.readAll();
			qDebug() << "Recv [" << data << "]";

			--count;
			if ( count == 0 )
			{
				qDebug() << "DBG break";
				m_stopFlag = true;
			}

		}
		else
		{
			QString date = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");

			qDebug() << "Send [" << date << "]";
			tcpSocket.write(date.toLocal8Bit());
		}

	}
	//tcpSocket.disconnectFromHost();
	tcpSocket.close();
	if ( tcpSocket.state() == QAbstractSocket::UnconnectedState || tcpSocket.waitForDisconnected() )
		qDebug() << "DBG disconnected";
	else
		qDebug() << "DBG waitForDisconnected() " << tcpSocket.error();


	qDebug() << "DBG tcpsocketthread finished";
#if 0
	   QByteArray block;
	   QDataStream out(&block, QIODevice::WriteOnly);
	   out.setVersion(QDataStream::Qt_4_0);
	   out << text;
	   tcpSocket.write(block);
#endif
}
