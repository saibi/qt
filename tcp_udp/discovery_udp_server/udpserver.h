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
	UdpServer(unsigned short port = 0, QObject *parent = nullptr);

protected:
	virtual void timerEvent(QTimerEvent *event);

protected slots:

	void slotReadPendingDatagrams();

protected:
	void processDatagram(QNetworkDatagram datagram);

private:

	QUdpSocket *m_udpSocket;
	QMap <QString, QDateTime> m_map;

	enum ConstantsList
	{
		DEFAULT_PORT = 8279,
		VALID_TIME = 60, // 60 sec
		CHECK_PERIOD = 10, // 10 sec


	};

};

#endif // UDPSERVER_H
