#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QMap>
#include <QDateTime>

class UdpServer : public QObject
{
	Q_OBJECT

public:
	UdpServer(QObject *parent = nullptr);

	bool startServer(unsigned short port = 0, unsigned int expire = 0);

signals:
	void signalError();

protected:
	virtual void timerEvent(QTimerEvent *event);

protected slots:
	void slotReadPendingDatagrams();

protected:
	void processDatagram(QNetworkDatagram datagram);

private:

	QUdpSocket *m_udpSocket;
	QMap <QString, QDateTime> m_map;
	unsigned int m_expire = 0;

	enum ConstantsList
	{
		DEFAULT_PORT = 8279,
		CHECK_PERIOD = 1, // sec
		DEFAULT_EXPIRE_TIME = 2, // min
	};

};

#endif // UDPSERVER_H
