#include "packet.h"

Packet::Packet()
{
    _packetType = PT_UNKNOWN;
    _crcPos = 0;
    _crcStatus = CS_NONE;
    _createdTime = QTime::currentTime();
}

Packet::Packet(int dataSize)
{
    _packetType = PT_UNKNOWN;
    _crcPos = 0;
    _crcStatus = CS_NONE;
    _data.reserve(dataSize);
    _createdTime = QTime::currentTime();
}

Packet::Packet(const char* pData, int dataSize, short packetType, int crcPos, short crcStatus)
{
    _packetType = packetType;
    _crcPos = crcPos;
    _crcStatus = crcStatus;
    _data.append(pData, dataSize);
    _createdTime = QTime::currentTime();
}

Packet::Packet(const Packet& packet)
{
    _packetType = packet._packetType;
    _crcPos = packet._crcPos;
    _crcStatus = packet._crcStatus;
    _data = packet._data;
    _createdTime = packet._createdTime;
}

Packet::~Packet()
{
}

Packet& Packet::operator=(const Packet& packet)
{
    _packetType = packet._packetType;
    _crcPos = packet._crcPos;
    _crcStatus = packet._crcStatus;
    _data = packet._data;
    _createdTime = packet._createdTime;
    return *this;
}

void Packet::set(const char *pData, int dataSize, short packetType, int crcPos, short crcStatus)
{
    _packetType = packetType;
    _crcPos = crcPos;
    _crcStatus = crcStatus;
    _data.clear();
    _data.append(pData, dataSize);
}

void Packet::reset()
{
    _packetType = PT_UNKNOWN;
    _crcPos = 0;
    _crcStatus = CS_NONE;
    _data.clear();
    _createdTime = QTime::currentTime();
}

int Packet::append(char c)
{
    _data.append(c);
    return 0;
}

int Packet::append(const char* pData, int dataSize)
{
    _data.append(pData, dataSize);
    return 0;
}

int Packet::append(const QByteArray& data)
{
    _data.append(data);
    return 0;
}

int Packet::appendNumber(int data)
{
    _data.append(QString::number(data));
    return 0;
}

int Packet::appendNumber(unsigned int data)
{
    _data.append(QString::number(data));
    return 0;
}

void Packet::_dump() const
{
    qDebug("================================================================================\n");
    qDebug("== Packet\n");
    qDebug("================================================================================\n");
    qDebug("%s\n", getAsHexString().toAscii().constData());
    qDebug("--------------------------------------------------------------------------------\n");
    qDebug("%s\n", getAsFormatString().toAscii().constData());
    qDebug("================================================================================\n\n");
}

QString Packet::getAsHexString() const
{
    QString hexStr;
    QByteArray hexArray = _data.toHex();

    for (int i = 0; i < hexArray.size(); i += 2)
    {
        if ((i % 32) == 0 && i > 0)
            hexStr += '\n';

        hexStr += hexArray.at(i);
        hexStr += hexArray.at(i+1);
        hexStr += ' ';
    }

    return hexStr;
}

QString Packet::getAsFormatString() const
{
    QString formatStr;
    unsigned char c;

    for (int i = 0; i < _data.size(); i++)
    {
        c = (unsigned char) _data.at(i);

        switch (c)
        {
            case RC_FS:     formatStr += "<FS>"; formatStr += '\n'; break;
            case RC_GS:     formatStr += "<GS>"; formatStr += '\n'; break;
            case RC_DC1:    formatStr += "<DC1>"; break;
            case RC_DC3:    formatStr += "<DC3>"; break;
            case RC_ACK:    formatStr += "<ACK>"; formatStr += '\n'; break;
            case RC_NAK:    formatStr += "<NAK>"; formatStr += '\n'; break;
            case RC_ESC:    formatStr += "<ESC>"; break;
            case RC_RS:     formatStr += "<RS>"; formatStr += '\n'; break;
            case RC_SUB:    formatStr += "<SUB>"; break;
            case RC_CR:     formatStr += "<CR>"; formatStr += '\n'; break;
            case RC_LF:     formatStr += "<LF>"; formatStr += '\n'; break;

            default:
                {
                    if (c > 127)
                        formatStr += '`';
                    else if (isprint(c))
                        formatStr += QChar(c);
                    else
                        formatStr += '`';
                }
        }
    }

    return formatStr;
}
