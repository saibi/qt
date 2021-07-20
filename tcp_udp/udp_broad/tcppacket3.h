#ifndef TCPPACKET3_H
#define TCPPACKET3_H

#include <QByteArray>
#include <QDebug>

class TcpPacket3
{
public:
	TcpPacket3();
	TcpPacket3(int flag, int type, const QByteArray & contents = "");
	TcpPacket3(const TcpPacket3 & packet);

	TcpPacket3& operator=(const TcpPacket3& packet);

	void set(int flag, int type, const QByteArray & contents = "");
	void set(const TcpPacket3& packet);

	enum TcpPacket3Flags
	{
		FLAG_NONE = 0,
		FLAG_BIT_ENCRYPTION = 0x01,
		FLAG_BIT_CHECKSUM = 0x80,
	};

	enum TcpPacket3Types
	{
		TYPE_NONE = 0,
		TYPE_CMDLINE,
		TYPE_SMALLFILE,
		TYPE_BIGFILE_START,
		TYPE_BIGFILE_FRAG,
		TYPE_BIGFILE_END,
	};

	enum Constants
	{
		VERSION = 1,

		HEADER_SIZE = 8,
		MAX_CONTENTS_SIZE = 50000,
		MAX_BIGFILE_FRAG_SIZE = 20000,
	};

	void clear();

	bool isValid() const { return m_valid; }

	int version() const;
	int flag() const;
	int type() const;
	int dataSize() const;
	int checksum() const;
	int orgSize() const;

	QByteArray contents();

	// to send packet
	const QByteArray & rawByteArray() const { return m_buf; }

	// build packet from bytearray
	int buildFromRawHeader(const QByteArray & raw);
	bool buildFromRawData(const QByteArray & data);

	static int containsTcpPacket3Prefix(const QByteArray & raw);

	bool verifyChecksum();


	// create packet
	bool setCmdLine(int flag, const QString & cmdline);
	bool setSmallFile(int flag, const QString & filename, const QByteArray & fileContents);

	static QList<TcpPacket3> makeFilePackets(int flag, const QString & filename, const QByteArray & fileContents);

	static int ccit_crc(const char *pData, int size);

	// debug
	friend QDebug operator<< (QDebug d, const TcpPacket3 &packet);
protected:

	QByteArray m_buf;
	bool m_valid;

	enum InternalConstants
	{
		HEADER_IDX_FS = 0,
		HEADER_IDX_MAGIC,
		HEADER_IDX_VER,
		HEADER_IDX_FLAG,
		HEADER_IDX_TYPE,
		HEADER_IDX_DATASIZE0,
		HEADER_IDX_DATASIZE1,
		HEADER_IDX_GS,

		DATA_IDX_CONTENTS0,
		DATA_IDX_CHECKSUM0 = DATA_IDX_CONTENTS0,

		CHECKSUM_LEN = 2,
		SIZE_LEN = 2,

		BIG_FILE_CHECKSUM_NONE = 0,
		BIG_FILE_CHECKSUM_CRC = 1, // not implemented

		RC_MAGIC = 0x42,
		RC_FS = 0x1c,
		RC_GS = 0x1d,

		HEADER_FIELD_MASK = 0xff,
	};

	static QByteArray encryptContents(const QByteArray & contents);
	static QByteArray decryptContents(const QByteArray & contents);
	static unsigned short calcChecksum(const QByteArray & buf);
	static unsigned short convert_buf2short(const char *buf);
	static void convert_short2buf(unsigned short val, char *buf);

	void fillDefaultHeader();
	unsigned short appendContents(int flag, const QByteArray & contents);

};

#endif // TcpPacket3_H
