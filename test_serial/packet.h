#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>
#include <QTime>

// Reserved Characters
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

#define RC_SEMICOLON	0x3B // ;
#define RC_EQUALS	0x3D // =
#define RC_COMMA	0x2C // ,
#define RC_ASTERISK	0x2A // *
#define RC_QUESTION	0x3F // ?

#define RC_QUOTATION1   0x22 // "
#define RC_QUOTATION2   0x27 // '

#define RC_DOUBLE_QUOTES    RC_QUOTATION1
#define RC_SINGLE_QUOTES    RC_QUOTATION2

//
#define RC_START_MSG	RC_FS
#define RC_END_MSG	RC_GS
#define RC_CRC_SEP	RC_RS
#define RC_RECORD_SEP	RC_CR
#define RC_RECORD_SEP2	RC_LF

#define RC_FIELD_SEP	RC_SEMICOLON
#define RC_LABEL_SEP	RC_EQUALS
#define RC_CODE_SEP	RC_COMMA

class Packet
{
public:
    enum PacketType
    {
        PT_UNKNOWN = 0,
        PT_ACK = 1,
        PT_NAK = 2,
        PT_DATA = 3,
    };

    enum CrcStatus
    {
        CS_NONE = 0,
        CS_OK = 1,
        CS_ERROR = 2,
    };

public:
    Packet();
    Packet(int dataSize);
    Packet(const char* pData, int dataSize, short packetType = PT_UNKNOWN, int crcPos = 0, short crcStatus = CS_NONE);
    Packet(const Packet& packet);

    ~Packet();

    Packet& operator=(const Packet& packet);

    void set(const char* pData, int dataSize, short packetType, int crcPos, short crcStatus);

    void reset();

    short getPacketType() const {return _packetType;}
    void setPacketType(short packetType) {_packetType = packetType;}

    QByteArray& getByteArray() {return _data;}

    const char* getData() const {return _data.constData();}

    int getSize() const {return _data.size();}

    int getCrcPosition() const {return _crcPos;}
    void setCrcPostion(int crcPos) {_crcPos = crcPos;}
    void markCrcPostion() {_crcPos = getSize();}

    short getCrcStatus() const {return _crcStatus;}
    void setCrcStatus(short crcStatus) {_crcStatus = crcStatus;}

    int append(char c);
    int append(const char* pData, int dataSize = -1);
    int append(const QByteArray& data);
    int append(const QString& data) {return append(data.toUtf8());}

    int appendNumber(int data);
    int appendNumber(unsigned int data);

    bool isEmpty() const {return _data.isEmpty();}

    void _dump() const;

    QString getAsHexString() const;

    QString getAsFormatString() const;

    const QTime& getCreatedTime() const {return _createdTime;}
    void setCreatedTime(const QTime& time = QTime::currentTime()) {_createdTime = time;}

protected:
    short _packetType;

    QByteArray _data;

    int _crcPos;

    short _crcStatus;

    QTime _createdTime;
};

#endif // PACKET_H
