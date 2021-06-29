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
	void incomingConnection(qintptr socketDescriptor) override;

};

#endif // TCPSERVER_H
