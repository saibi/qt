#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpServer : public QObject
{
	Q_OBJECT

public:
	UdpServer(unsigned short port = 0, QObject *parent = nullptr);

protected slots:

	void slotReadPendingDatagrams();

protected:
	void processDatagram(QNetworkDatagram datagram);

private:

	QUdpSocket *m_udpSocket;

	enum ConstantsList
	{
		DEFAULT_PORT = 8279,

	};
};

#endif // UDPSERVER_H
