#ifndef TCPSOCKETTHREAD_H
#define TCPSOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>

#include <QMutex>
#include <QQueue>

#include "tcppacket2.h"

class TcpSocketThread : public QThread
{
	Q_OBJECT

public:
	TcpSocketThread(int socketDescriptor, QObject * parent );

	void run() override;

	void terminate();

	QHostAddress clientAddress() const;

	void sendPacket(const TcpPacket2 & packet);
	bool recvPacket(TcpPacket2 & packet);

signals:
	void signalError(QTcpSocket::SocketError socketError);

private:
	int m_socketDescriptor;
	bool m_stopFlag = false;

	QByteArray m_recvBuf;

	QMutex m_recvMutex;
	QMutex m_sendMutex;

	QQueue <TcpPacket2> m_recvQ;
	QQueue <TcpPacket2> m_sendQ;
};

#endif // TCPSOCKETTHREAD_H
