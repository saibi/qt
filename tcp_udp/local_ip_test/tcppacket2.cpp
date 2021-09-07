#include "tcppacket2.h"

TcpPacket2::TcpPacket2()
{
	clear();
}

TcpPacket2::TcpPacket2(int flag, int cmd, const QByteArray & contents)
{
	set(flag, cmd, contents);
}

TcpPacket2::TcpPacket2(const TcpPacket2 & packet)
{
	set(packet);
}

TcpPacket2& TcpPacket2::operator=(const TcpPacket2& packet)
{
	set(packet);

	return *this;
}

void TcpPacket2::clear()
{
	m_buf.clear();
	m_valid = false;
}

void TcpPacket2::set(const TcpPacket2 & packet)
{
	m_valid = packet.m_valid;
	m_buf = packet.m_buf;
}

void TcpPacket2::fillDefaultHeader()
{
	m_buf.fill(0, HEADER_SIZE);
	m_buf[HEADER_IDX_FS] = RC_FS;
	m_buf[HEADER_IDX_MAGIC] = RC_MAGIC;
	m_buf[HEADER_IDX_VER] = VERSION;
	m_buf[HEADER_IDX_FLAG] = FLAG_NONE;
	m_buf[HEADER_IDX_CMD] = CMD_NONE;
	m_buf[HEADER_IDX_GS] = RC_GS;
}

void TcpPacket2::set(int flag, int cmd, const QByteArray & contents)
{
	fillDefaultHeader();

	m_buf[HEADER_IDX_FLAG] = (char)(flag & MASK_FLAG);
	m_buf[HEADER_IDX_CMD] = (char)(cmd & MASK_CMD);

	if ( contents.size() > 0 && contents.size() <= MAX_CONTENTS_SIZE )
	{
		union {
			unsigned char buf[2];
			unsigned short val;
		} dataSize;

		dataSize.val = appendCheckSumAndContents(contents);

		m_buf[HEADER_IDX_DATASIZE0] = dataSize.buf[0];
		m_buf[HEADER_IDX_DATASIZE1] = dataSize.buf[1];
	}
	m_valid = true;
}

/// \return data_size
int TcpPacket2::appendCheckSumAndContents(const QByteArray & contents)
{
	QByteArray checksum;

	// calc checksum - CHECKSUM_NONE
	checksum.fill(0, 2);

	m_buf.append(checksum);
	m_buf.append(contents);

	return contents.size() + 2;
}

QByteArray TcpPacket2::contents() const
{
	if ( ! m_valid )
		return QByteArray();

	int type = m_buf.at(DATA_IDX_CHECKSUM_TYPE);

	if ( type != CHECKSUM_NONE )
	{
		int size = m_buf.at(DATA_IDX_CHECKSUM_SIZE);

		return m_buf.mid(DATA_IDX_CHECKSUM_START + size);
	}
	return m_buf.mid(DATA_IDX_NONE_CHECKSUM_CONTENTS_START);
}

int TcpPacket2::flag() const
{
	if ( m_valid )
		return (int)(unsigned char)m_buf.at(HEADER_IDX_FLAG);

	return 0;
}

int TcpPacket2::cmd() const
{
	if ( m_valid )
		return (int)(unsigned char)m_buf.at(HEADER_IDX_CMD);

	return 0;
}


/// \return dataSize valid TcpPacket2 header with additional data
/// \return 0 valid TcpPacket2 header. (header only packet)
/// \return -1 invalid TcpPacket2
int TcpPacket2::buildFromRawHeader(const QByteArray &raw)
{
	if ( raw.at(HEADER_IDX_FS) == RC_FS && raw.at(HEADER_IDX_MAGIC) == RC_MAGIC && raw.at(HEADER_IDX_GS) == RC_GS )
	{
		union {
			unsigned char buf[2];
			unsigned short val;
		} dataSize;

		dataSize.buf[0] = raw.at(HEADER_IDX_DATASIZE0);
		dataSize.buf[1] = raw.at(HEADER_IDX_DATASIZE1);

		clear();
		m_buf = raw.left(HEADER_SIZE);

		if ( dataSize.val == 0 )
			m_valid = true;

		return dataSize.val;
	}
	return -1;
}

// use with buildFromRawHeader
int TcpPacket2::containsTcpPacket2Prefix(const QByteArray &raw)
{
    const char prefix[] = { RC_FS, RC_MAGIC };

	int idx = raw.indexOf(prefix);
	if ( idx < 0 )
		idx = raw.indexOf(RC_FS); // if not found, search RC_FS only

	if ( idx == 0 )
		return -1; // buildFromRawHeader() already searched from index 0.

	return idx;
}

bool TcpPacket2::fillRawData(const QByteArray &data)
{
	if ( dataSize() == data.size() )
	{
		// TO-DO : checksum
		//	int checksumType = m_buf.at(DATA_IDX_CHECKSUM_TYPE);
		//	int checksumSize = m_buf.at(DATA_IDX_CHECKSUM_SIZE);
		m_buf += data;
		m_valid = true;
		return true;
	}
	return false;
}

int TcpPacket2::dataSize() const
{
	union {
		unsigned char buf[2];
		unsigned short val;
	} dataSize;

	dataSize.buf[0] = m_buf.at(HEADER_IDX_DATASIZE0);
	dataSize.buf[1] = m_buf.at(HEADER_IDX_DATASIZE1);

	return dataSize.val;
}

QDebug operator<< (QDebug d, const TcpPacket2 &packet)
{
	if ( packet.isValid() )
	{
		if ( packet.dataSize() > 0 )
			d.nospace() << "[" << packet.flag() << "|" << packet.cmd() << "|" << packet.dataSize() << "]" << packet.rawData().mid(TcpPacket2::HEADER_SIZE, 40);
		else
			d.nospace() << "[" << packet.flag() << "|" << packet.cmd() << "]";
	}
	else
		d.nospace() << "invalid TcpPacket2";

  return d;
}
