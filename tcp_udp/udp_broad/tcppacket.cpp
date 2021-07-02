#include "tcppacket.h"

#include <QList>

#define PREFIX_HEADER_ONLY "ewh "
#define PREFIX_HAVE_DATA "ewd "
#define EMPTY_CHECKSUM_FIELD " d "


TcpPacket::TcpPacket()
{
	clear();
}

TcpPacket::TcpPacket(const QString & cmd)
{
	set(cmd);
}

TcpPacket::TcpPacket(const QString & cmd, const QByteArray & data)
{
	set(cmd, data);
}

TcpPacket::TcpPacket(const TcpPacket & packet)
{
	set(packet);
}

TcpPacket& TcpPacket::operator=(const TcpPacket& packet)
{
	set(packet);
	return *this;
}


void TcpPacket::clear()
{
	m_type = TCP_PACKET_UNKNOWN;
	m_header.clear();
	m_data.clear();
}

void TcpPacket::set(const QString & cmd, const QByteArray & data)
{
	QString header;

	if ( data.isEmpty() )
	{
		m_type = TCP_PACKET_HEADER_ONLY;
		header = PREFIX_HEADER_ONLY + cmd;
		m_data.clear();
	}
	else
	{
		m_type = TCP_PACKET_HAVE_DATA;
		header = PREFIX_HAVE_DATA + QString::number(data.size()) + EMPTY_CHECKSUM_FIELD + cmd;
		m_data = data;
	}

	m_header.fill(0, HEADER_SIZE);
	qstrncpy(m_header.data(), header.toLocal8Bit().constData(), HEADER_SIZE);
	m_header.truncate(HEADER_SIZE);
}

void TcpPacket::set(const TcpPacket & packet)
{
	m_type = packet.m_type;
	m_header = packet.m_header;
	m_data = packet.m_data;
}

int TcpPacket::fromByteArray(const QByteArray &raw)
{
	QByteArray prefix = raw.left(PREFIX_LEN);
	int dataSize = 0;

	clear();

	if ( prefix.compare(PREFIX_HEADER_ONLY) == 0 )
	{
		m_type = TCP_PACKET_HEADER_ONLY;
	}
	else if ( prefix.compare(PREFIX_HAVE_DATA) == 0 )
	{
		QList <QByteArray> list = raw.split(' ');

		if ( list.size() < 2 )
			return -1;

		m_type = TCP_PACKET_HAVE_DATA;

		dataSize = list.at(1).toInt();
	}
	else
	{
		return -1;
	}

	m_header = raw;
	m_header.truncate(HEADER_SIZE);
	return dataSize;
}

bool TcpPacket::fillData(const QByteArray& data)
{
	if ( m_type == TCP_PACKET_HAVE_DATA )
	{
		m_data = data;
		return true;
	}
	return false;
}
