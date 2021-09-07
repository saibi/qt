#ifndef TCPPACKET2_H
#define TCPPACKET2_H

#include <QByteArray>
#include <QDebug>

class TcpPacket2
{
public:
	TcpPacket2();
	TcpPacket2(int flag, int cmd, const QByteArray & contents = "");
	TcpPacket2(const TcpPacket2 & packet);

	TcpPacket2& operator=(const TcpPacket2& packet);

	void set(int flag, int cmd, const QByteArray & contents = "");
	void set(const TcpPacket2& packet);

	enum TcpPacket2Flags
	{
		FLAG_NONE = 0,
	};

	enum TcpPacket2Commands
	{
		CMD_NONE = 0,
	};

	enum Constants
	{
		VERSION = 1,
		HEADER_SIZE = 8,
		MAX_CONTENTS_SIZE = 65535 - 255 - 4,

	};

	enum CheckSumType
	{
		CHECKSUM_NONE = 0,
	};

	void clear();

	bool isValid() const { return m_valid; }
	int flag() const;
	int cmd() const;
	int dataSize() const;

	QByteArray contents() const;
	const QByteArray & rawData() const { return m_buf; }

	int buildFromRawHeader(const QByteArray & raw);
	bool fillRawData(const QByteArray & data);

	static int containsTcpPacket2Prefix(const QByteArray & raw);


	friend QDebug operator<< (QDebug d, const TcpPacket2 &packet);

protected:

	QByteArray m_buf;
	bool m_valid;

	enum InternalConstants
	{
		HEADER_IDX_FS = 0,
		HEADER_IDX_MAGIC,
		HEADER_IDX_VER,
		HEADER_IDX_FLAG,
		HEADER_IDX_CMD,
		HEADER_IDX_DATASIZE0,
		HEADER_IDX_DATASIZE1,
		HEADER_IDX_GS,

		DATA_IDX_CHECKSUM_TYPE,
		DATA_IDX_CHECKSUM_SIZE,
		DATA_IDX_CHECKSUM_START,
		DATA_IDX_NONE_CHECKSUM_CONTENTS_START = DATA_IDX_CHECKSUM_START,

		RC_MAGIC = 0x42,
		RC_FS = 0x1c,
		RC_GS = 0x1d,

		MASK_FLAG = 0xff,
		MASK_CMD = 0xff,
	};

	void fillDefaultHeader();
	int appendCheckSumAndContents(const QByteArray & contents);
};


#endif // TCPPACKET2_H
