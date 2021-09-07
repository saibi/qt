#include "tcpserver.h"


TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	TcpSocketThread *thread = new TcpSocketThread(socketDescriptor, this);
	connect(thread, &TcpSocketThread::finished, this, &TcpServer::signalDisconnected);
	connect(thread, &TcpSocketThread::finished, thread, &TcpSocketThread::deleteLater);
	emit signalConnected(thread);
}
