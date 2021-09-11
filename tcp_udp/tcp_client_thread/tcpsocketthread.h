#ifndef TCPSOCKETTHREAD_H
#define TCPSOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>

#include <QMutex>
#include <QQueue>

//#include "tcppacket3.h"

class TcpSocketThread : public QThread
{
	Q_OBJECT

public:
	TcpSocketThread(int socketDescriptor, QObject * parent );
	TcpSocketThread(const QHostAddress & address, quint16 port, QObject * parent);

	void run() override;

//	void terminate();

//	QHostAddress clientAddress() const;

//	void sendPacket(const TcpPacket3 & packet);
//	bool recvPacket(TcpPacket3 & packet);

signals:
	void signalError(QTcpSocket::SocketError socketError);

protected slots:
	void slot_disconnected();
	void slot_socketError(QAbstractSocket::SocketError socketError);
	void slot_stateChanged(QAbstractSocket::SocketState socketState);

private:
	bool m_connectMode;

	int m_socketDescriptor;
	QHostAddress m_address;
	quint16 m_port;

	bool m_stopFlag;


//	QByteArray m_recvBuf;

	QMutex m_recvMutex;
	QMutex m_sendMutex;

//	QQueue <TcpPacket3> m_recvQ;
//	QQueue <TcpPacket3> m_sendQ;


//	int sendPacketData(QTcpSocket * psocket, const TcpPacket3 & packet);
};

#endif // TCPSOCKETTHREAD_H
