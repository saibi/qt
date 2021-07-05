#ifndef TCPSOCKETTHREAD_H
#define TCPSOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>

class TcpSocketThread : public QThread
{
	Q_OBJECT

public:
	TcpSocketThread(int socketDescriptor, QObject * parent );

	void run() override;

	void sendTest();
	void terminate();

	QHostAddress clientAddress() const;

	void setTransfer(int val) { m_transfer = val; }
	void setRepeat(int val) { m_repeat = val; }
signals:
	void signalError(QTcpSocket::SocketError socketError);

private:
	int m_socketDescriptor;
	bool m_stopFlag = false;
	int m_transfer = 1000;
	int m_repeat = 10;

	bool m_sendTest = false;
};

#endif // TCPSOCKETTHREAD_H
