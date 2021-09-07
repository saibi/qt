#ifndef TCPSERVER_H
#define TCPSERVER_H


#include <QTcpServer>

#include "tcpsocketthread.h"

class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(QObject *parent = nullptr);

signals:
	void signalConnected(TcpSocketThread *thread);
	void signalDisconnected();

protected:
#if QT_VERSION >= 0x050800
	void incomingConnection(qintptr socketDescriptor) override;
#else
	void incomingConnection(int socketDescriptor) override;
#endif

};

#endif // TCPSERVER_H
