#include "tcpserver.h"


TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

}

#if QT_VERSION >= 0x050800
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	TcpSocketThread *thread = new TcpSocketThread(socketDescriptor, this);
	connect(thread, &TcpSocketThread::finished, this, &TcpServer::signalDisconnected);
	connect(thread, &TcpSocketThread::finished, thread, &TcpSocketThread::deleteLater);
	emit signalConnected(thread);
}
#else
void TcpServer::incomingConnection(int socketDescriptor)
{
	TcpSocketThread *thread = new TcpSocketThread(socketDescriptor, this);
	connect(thread, SIGNAL(finished()), this, SLOT(signalDisconnected()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	emit signalConnected(thread);
}
#endif
