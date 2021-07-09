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
		char sizeBuf[SIZE_LEN];

		convert_short2buf(appendContents(flag, contents), sizeBuf);
		m_buf[HEADER_IDX_DATASIZE0] = sizeBuf[0];
		m_buf[HEADER_IDX_DATASIZE1] = sizeBuf[1];
	}
	m_valid = true;
}

/// \return data_size
unsigned short TcpPacket3::appendContents(int flag, const QByteArray & contents)
{
	if ( (flag & (FLAG_BIT_ENCRYPTION | FLAG_BIT_CHECKSUM)) == 0 )
	{
		m_buf.append(contents);
		return contents.size();
	}

	QByteArray buf;
	int dataSize = contents.size();

	if ( flag & FLAG_BIT_ENCRYPTION )
	{
		char sizeBuf[SIZE_LEN];

		buf = encryptContents(contents);

		convert_short2buf(contents.size(), sizeBuf);
		buf.prepend(sizeBuf, SIZE_LEN);
		dataSize = buf.size();
	}
	else
	{
		buf = contents;
	}

	if ( flag & FLAG_BIT_CHECKSUM )
	{
		char checksumBuf[CHECKSUM_LEN];

		convert_short2buf(calcChecksum(buf), checksumBuf);
		buf.prepend(checksumBuf, CHECKSUM_LEN);
		dataSize += CHECKSUM_LEN;
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

	if ( (type & FLAG_BIT_CHECKSUM) && (dataSize() > 0 ) )
	{
		unsigned short checksum = convert_buf2short(&m_buf.data()[DATA_IDX_CHECKSUM0]);

		qDebug("DBG checksum %d, calc %d", checksum, calcChecksum(m_buf.mid(DATA_IDX_CHECKSUM0 + CHECKSUM_LEN)) );
		return (checksum == calcChecksum(m_buf.mid(DATA_IDX_CHECKSUM0 + CHECKSUM_LEN)));
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
		clear();
		m_buf = raw.left(HEADER_SIZE);
		unsigned short dataSize = convert_buf2short(&raw.data()[HEADER_IDX_DATASIZE0]);

		if ( dataSize == 0 )
			m_valid = true;

		return dataSize;
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

int TcpPacket3::dataSize() const
{
	return convert_buf2short(&m_buf.data()[HEADER_IDX_DATASIZE0]);
}

int TcpPacket3::checksum() const
{
	if ( flag() & FLAG_BIT_CHECKSUM )
		return convert_buf2short(&m_buf.data()[DATA_IDX_CONTENTS0]);

	return 0;
}

int TcpPacket3::orgSize() const
{
	if ( flag() & FLAG_BIT_ENCRYPTION )
	{
		if ( flag() & FLAG_BIT_CHECKSUM )
			return convert_buf2short(&m_buf.data()[DATA_IDX_CONTENTS0 + CHECKSUM_LEN]);

		return convert_buf2short(&m_buf.data()[DATA_IDX_CONTENTS0]);
	}
	return 0;
}


QDebug operator<< (QDebug d, const TcpPacket3 &packet)
{
	if ( packet.isValid() )
	{
		if ( packet.dataSize() > 0 )
			d.nospace() << "[" << packet.version() << "|" << packet.flag() << "|" << packet.type() << "|" << packet.dataSize() << "]" << "[" << packet.checksum() << "|" << packet.orgSize() << "]" << packet.rawByteArray().mid(TcpPacket3::HEADER_SIZE, 60);
		else
			d.nospace() << "[" << packet.version() << "|" << packet.flag() << "|" << packet.type() << "]";
	}
	else
		d.nospace() << "invalid TcpPacket3";

  return d;
}


bool TcpPacket3::setCmdLine(int flag, const QString & cmdline)
{
	if ( cmdline.size() > MAX_CONTENTS_SIZE )
	{
		qDebug("DBG too long cmdline");
		return false;
	}

	set(flag, TYPE_CMDLINE, cmdline.toLocal8Bit());
	return true;
}

bool TcpPacket3::setSmallFile(int flag, const QString &filename, const QByteArray &fileContents)
{
	if ( fileContents.size() > MAX_CONTENTS_SIZE )
	{
		qDebug("DBG file is too big");
		return false;
	}

	QByteArray contents(filename.toLocal8Bit());
	contents += '\0';

	char sizeBuf[SIZE_LEN];
	convert_short2buf(fileContents.size(), sizeBuf);
	contents.append(sizeBuf, SIZE_LEN);

	contents += fileContents;
	set(flag, TYPE_SMALLFILE, contents);
	return true;
}

QList<TcpPacket3> TcpPacket3::makeFilePackets(int flag, const QString &filename, const QByteArray &fileContents)
{
	QList<TcpPacket3> list;
	TcpPacket3 packet;

	// check small file
#if 0
	if ( fileContents.size() <= MAX_CONTENTS_SIZE )
	{
		if ( packet.setSmallFile(flag, filename, fileContents) )
			list.append(packet);

		return list;
	}
#endif

	// big file start

	char id = ' ' + (fileContents.size() % 90);
	int fileSize = fileContents.size();
	int fragmentSize = 10; // MAX_CONTENTS_SIZE;
	int remainSize = fileSize % fragmentSize;
	int fragmentCount = fileSize / fragmentSize;

	qDebug("DBG id %c, fileSize %d, fragmentSize %d, remainSize %d, fragmentCount %d", id, fileSize, fragmentSize, remainSize, fragmentCount);

	QByteArray contents;

	contents.append(id);
	contents.append(filename.toLocal8Bit());
	contents.append('\0');
	contents.append(QString::number(fileSize).toLocal8Bit());
	contents.append('\0');
	contents.append(QString::number(fragmentCount + (remainSize > 0 ? 1 : 0)).toLocal8Bit());
	contents.append('\0');

	packet.set(flag, TYPE_BIGFILE_START, contents);
	list.append(packet);


	// big file fragment

	char buf[SIZE_LEN];
	convert_short2buf(fragmentSize, buf);

	for ( int i = 0 ; i < fragmentCount ; ++i )
	{

		contents.clear();
		contents.append(id);
		contents.append(QString::number(i).toLocal8Bit());
		contents.append('\0');
		contents.append(buf, SIZE_LEN);
		contents.append(fileContents.mid(fragmentSize * i, fragmentSize));

		packet.set(flag, TYPE_BIGFILE_FRAG, contents );
		list.append(packet);
	}

	if ( remainSize > 0 )
	{
		convert_short2buf(remainSize, buf);
		contents.clear();
		contents.append(id);
		contents.append(QString::number(fragmentCount).toLocal8Bit());
		contents.append('\0');
		contents.append(buf, SIZE_LEN);
		contents.append(fileContents.mid(fragmentSize * fragmentCount, remainSize));

		packet.set(flag, TYPE_BIGFILE_FRAG, contents );
		list.append(packet);
	}

	// big file end

	char checksumType = BIG_FILE_CHECKSUM_NONE;

	contents.clear();
	contents.append(id);
	contents.append(checksumType);

	packet.set(flag, TYPE_BIGFILE_END, contents);
	list.append(packet);

	return list;
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
	return (unsigned char)data.at(0) + 0x0100; // test checksum
}

unsigned short TcpPacket3::convert_buf2short(const char *buf)
{
	union {
		char buf[2];
		unsigned short val;
	} conv;

	conv.buf[0] = buf[0];
	conv.buf[1] = buf[1];

	return conv.val;
}

void TcpPacket3::convert_short2buf(unsigned short val, char *buf)
{
	union {
		char buf[2];
		unsigned short val;
	} conv;

	conv.val = val;
	buf[0] = conv.buf[0];
	buf[1] = conv.buf[1];
}
