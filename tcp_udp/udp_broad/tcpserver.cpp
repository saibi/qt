#include "tcpserver.h"


TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	TcpSocketThread *thread = new TcpSocketThread(socketDescriptor, this);
	connect(thread, &TcpSocketThread::finished, thread, &TcpSocketThread::deleteLater);
	thread->start();
	emit signalTcpSocketThreadCreated(thread);
}
