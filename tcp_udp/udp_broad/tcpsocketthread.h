#ifndef TCPSOCKETTHREAD_H
#define TCPSOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>

class TcpSocketThread : public QThread
{
	Q_OBJECT

public:
	TcpSocketThread(int socketDescriptor, QObject * parent );

	void run() override;

signals:
	void signalError(QTcpSocket::SocketError socketError);

private:
	int m_socketDescriptor;
	bool m_stopFlag = false;

};

#endif // TCPSOCKETTHREAD_H
