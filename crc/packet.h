#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>

#define RC_FS	0x1C // Start of message
#define RC_GS	0x1D // End of message
#define RC_DC1	0x11 // Reserved (XOFF)
#define RC_DC3	0x13 // Reserved (XON)
#define RC_ACK	0x06 // Positive acknowledgement
#define RC_NAK	0x15 // Negative acknowledgement
#define RC_ESC	0x1B // Escape
#define RC_RS	0x1E // CRC separator
#define RC_SUB	0x1A // DOS End-of-file marker
#define RC_CR	0x0D // Record separator
#define RC_LF	0x0A // Record separator

#define RC_LABEL_SEP 0x3D // =

class Packet
{
public:
	Packet();
	Packet(const QString & text);

	void setText(const QString & text);

	QString getAsFormatString();

	static QString toFormatString(const QByteArray & data);
	static int crc16ccitt(const char * pData, int size);

	bool calcCrc(bool autoErrorCorrection = false);

	int m_crcVal;
	QString m_crcStr;

private:
	QString m_text;
	QByteArray m_data;

	void convertBin();
};

#endif // PACKET_H
