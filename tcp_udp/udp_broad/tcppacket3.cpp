#include "tcppacket3.h"

TcpPacket3::TcpPacket3()
{
	clear();
}

TcpPacket3::TcpPacket3(int flag, int cmd, const QByteArray & contents)
{
	set(flag, cmd, contents);
}

TcpPacket3::TcpPacket3(const TcpPacket3 & packet)
{
	set(packet);
}

TcpPacket3& TcpPacket3::operator=(const TcpPacket3& packet)
{
	set(packet);

	return *this;
}

void TcpPacket3::clear()
{
	m_buf.clear();
	m_valid = false;
}

void TcpPacket3::set(const TcpPacket3 & packet)
{
	m_valid = packet.m_valid;
	m_buf = packet.m_buf;
}

void TcpPacket3::fillDefaultHeader()
{
	m_buf.fill(0, HEADER_SIZE);
	m_buf[HEADER_IDX_FS] = RC_FS;
	m_buf[HEADER_IDX_MAGIC] = RC_MAGIC;
	m_buf[HEADER_IDX_VER] = VERSION;
	m_buf[HEADER_IDX_FLAG] = FLAG_NONE;
	m_buf[HEADER_IDX_TYPE] = TYPE_NONE;
	m_buf[HEADER_IDX_GS] = RC_GS;
}

void TcpPacket3::set(int flag, int type, const QByteArray & contents)
{
	fillDefaultHeader();

	m_buf[HEADER_IDX_FLAG] = (char)(flag & HEADER_FIELD_MASK);
	m_buf[HEADER_IDX_TYPE] = (char)(type & HEADER_FIELD_MASK);

	if ( contents.size() > 0 && contents.size() <= MAX_CONTENTS_SIZE )
	{
		union {
			unsigned char buf[2];
			unsigned short val;
		} dataSize;

		dataSize.val = appendContents(flag, contents);

		m_buf[HEADER_IDX_DATASIZE0] = dataSize.buf[0];
		m_buf[HEADER_IDX_DATASIZE1] = dataSize.buf[1];
	}
	m_valid = true;
}

/// \return encrypted contents
QByteArray TcpPacket3::encryptContents(const QByteArray & contents)
{
	// TO-DO : encrypt

	QByteArray enc = contents;

	enc.prepend("encrypt "); // test encryption
	return enc;
}

/// \return decrypted contents
QByteArray TcpPacket3::decryptContents(const QByteArray & contents)
{
	// TO-DO : decrypt

	return contents.mid(8); // test decryption
}

unsigned short TcpPacket3::calcChecksum(const QByteArray & data)
{
	// TO-DO : caculate checksum

	return data.at(0) + 0x0100; // test checksum
}

/// \return data_size
int TcpPacket3::appendContents(int flag, const QByteArray & contents)
{
	if ( (flag & (FLAG_BIT_ENCRYPTION | FLAG_BIT_CHECKSUM)) == 0 )
	{
		m_buf.append(contents);
		return contents.size();
	}

	QByteArray buf;
	union {
		char buf[2];
		unsigned short val;
	} conv;
	int dataSize = contents.size();

	if ( flag & FLAG_BIT_ENCRYPTION )
	{
		conv.val = contents.size();

		buf = encryptContents(contents);
		buf.prepend(conv.buf, 2);
		dataSize = buf.size();
	}
	else
	{
		buf = contents;
	}

	if ( flag & FLAG_BIT_CHECKSUM )
	{
		conv.val = calcChecksum(buf);
		buf.prepend(conv.buf, 2);
		dataSize += 2;
	}

	m_buf.append(buf);
	return dataSize;
}

/// \return contents
QByteArray TcpPacket3::contents()
{
	if ( ! m_valid )
		return QByteArray();

	int type = (int)(unsigned char)m_buf[HEADER_IDX_FLAG];
	int idx = DATA_IDX_CONTENTS0;

	if ( type & FLAG_BIT_CHECKSUM )
		idx += 2;

	if ( type & FLAG_BIT_ENCRYPTION )
	{
		idx += 2;
		return decryptContents(m_buf.mid(idx));
	}
	else
	{
		return m_buf.mid(idx);
	}

}

bool TcpPacket3::verifyChecksum()
{
	int type = (int)(unsigned char)m_buf[HEADER_IDX_FLAG];

	if ( type & FLAG_BIT_CHECKSUM )
	{
		union {
			char buf[2];
			unsigned short val;
		} conv;

		conv.buf[0] = m_buf[0];
		conv.buf[1] = m_buf[1];
		return (conv.val == calcChecksum(m_buf.mid(2)));
	}
	return true;
}

int TcpPacket3::version() const
{
	if ( m_valid )
		return (int)(unsigned char)m_buf.at(HEADER_IDX_VER);

	return 0;
}

int TcpPacket3::flag() const
{
	if ( m_valid )
		return (int)(unsigned char)m_buf.at(HEADER_IDX_FLAG);

	return 0;
}

int TcpPacket3::type() const
{
	if ( m_valid )
		return (int)(unsigned char)m_buf.at(HEADER_IDX_TYPE);

	return 0;
}


/// \return dataSize valid TcpPacket3 header with additional data
/// \return 0 valid TcpPacket3 header. (header only packet)
/// \return -1 invalid TcpPacket3
int TcpPacket3::buildFromRawHeader(const QByteArray &raw)
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

/// call after buildFromRawHeader if dataSize > 0
bool TcpPacket3::buildFromRawData(const QByteArray &data)
{
	if ( dataSize() == data.size() )
	{
		m_buf += data;
		m_valid = true;
		return true;
	}
	return false;
}

/// if buildFromRawHeader returns -1, call this function to check raw header contains packet prefix chararacters.
int TcpPacket3::containsTcpPacket3Prefix(const QByteArray &raw)
{
	QByteArray prefix = { RC_FS, RC_MAGIC };

	int idx = raw.indexOf(prefix);
	if ( idx < 0 )
		idx = raw.indexOf(RC_FS); // if not found, search RC_FS only

	if ( idx == 0 )
		return -1; // buildFromRawHeader() already searched from index 0.

	return idx;
}


int TcpPacket3::dataSize() const
{
	union {
		unsigned char buf[2];
		unsigned short val;
	} dataSize;

	dataSize.buf[0] = m_buf.at(HEADER_IDX_DATASIZE0);
	dataSize.buf[1] = m_buf.at(HEADER_IDX_DATASIZE1);

	return dataSize.val;
}

QDebug operator<< (QDebug d, const TcpPacket3 &packet)
{
	if ( packet.isValid() )
	{
		if ( packet.dataSize() > 0 )
			d.nospace() << "[" << packet.version() << "|" << packet.flag() << "|" << packet.type() << "|" << packet.dataSize() << "]" << packet.rawByteArray().mid(TcpPacket3::HEADER_SIZE, 40);
		else
			d.nospace() << "[" << packet.version() << "|" << packet.flag() << "|" << packet.type() << "]";
	}
	else
		d.nospace() << "invalid TcpPacket3";

  return d;
}
