#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <QByteArray>
#include <QString>

class TcpPacket
{
public:
	TcpPacket();
	TcpPacket(const QString & cmd);
	TcpPacket(const QString & cmd, const QByteArray & data = "");

	TcpPacket(const TcpPacket & packet);

	TcpPacket& operator=(const TcpPacket& packet);

	void set(const QString &cmd, const QByteArray & data = "");
	void set(const TcpPacket& packet);

	enum TcpPacketType
	{
		TCP_PACKET_UNKNOWN = 0,
		TCP_PACKET_HEADER_ONLY,
		TCP_PACKET_HAVE_DATA,

	};

	enum Constants
	{
		HEADER_SIZE = 80,
	};

	void clear();

	int type() const { return m_type; }
	const QByteArray & header() const { return m_header; }
	const QByteArray & data() const { return m_data; }

	int fromByteArray(const QByteArray & raw);
	bool fillData(const QByteArray & data);

protected:
	int m_type;

	QByteArray m_header;
	QByteArray m_data;

	enum InternalConstants
	{
		PREFIX_LEN = 4,
	};
};

#endif // TCPPACKET_H
