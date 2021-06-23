#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <QByteArray>

class TcpPacket
{
public:
	TcpPacket();

	int size();
	int headerSize();
	int dataSize();


protected:
	QByteArray header;
	QByteArray data;
};

#endif // TCPPACKET_H
