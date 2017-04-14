#include "parser.h"

#include <QByteArray>

#include "packet.h"
#include "record.h"
#include "recordset.h"

Parser::Parser()
{
}

int Parser::crc16ccitt(char* pData, int size)
{
    quint16 crc16 = 0;

    if (pData && size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            crc16 = (crc16 >> 8) | (crc16 << 8);
            crc16 ^= (quint8) pData[i];
            crc16 ^= (crc16 & 0xFF) >> 4;
            crc16 ^= (crc16 << 8) << 4;
            crc16 ^= ((crc16 & 0xFF) << 4) << 1;
        }
    }
    else
    {
        return -1;
    }

    return (int) crc16;
}

bool Parser::parseRecord(const char* pData, int size, Record& record)
{
    int labelSepPos = 0;

    // <record label><label separator><field value>[<field separator><field value> ¡¦ ]
    // EXAMPLE: SPH=2.50;2.75

    for (int i = 0; i < size; i++)
    {
        if (RC_LABEL_SEP == pData[i])
        {
            labelSepPos = i;
            break;
        }
    }

    if (labelSepPos <= 0 || labelSepPos >= size)
        return false;

    record.setKey(pData, labelSepPos);
    record.setValue(pData + labelSepPos + 1, size - labelSepPos - 1);

    return true;
}

bool Parser::parseRecord(const QString& data, Record& record)
{
    QByteArray tmp = data.toUtf8();
    return parseRecord(tmp.constData(), tmp.size(), record);
}

bool Parser::parsePacketToRecordSet(const Packet& packet, RecordSet& recordSet)
{
    const char* pData = packet.getData();
    if (pData == 0)
        return false;

    int crcPos = packet.getCrcPosition();
    if (crcPos == 0)
        crcPos = packet.getSize();

    QByteArray buffer;
    char c;
    Record record;

    recordSet.reset();

    for (int i = 0; i < crcPos; i++)
    {
        c = (char) (pData[i]);

        if (RC_RECORD_SEP == c || RC_RECORD_SEP2 == c || RC_CRC_SEP == c || RC_START_MSG == c || RC_END_MSG == c)
        {
            if (buffer.size() > 0)
            {
                if (parseRecord(buffer.constData(), buffer.size(), record))
                    recordSet.append(record);

                buffer.clear();
            }

            continue;
        }

        buffer.append(c);
    }

    return true;
}

bool Parser::makePacketFromRecordSet(Packet& packet, const RecordSet& recordSet, bool crcSepFlag)
{
    packet.reset();

    packet.append(RC_START_MSG); // <FS>

    for (int i = 0; i < recordSet.getSize(); i++)
    {
        const Record& record = recordSet.getAt(i);

        packet.append(record.getKeyByteArray());
        packet.append(RC_LABEL_SEP); // =
        packet.append(record.getValueByteArray());
        packet.append(RC_RECORD_SEP); // <CR>
    }

    if (crcSepFlag)
        packet.append(RC_CRC_SEP); // <RS>

    int crcVerified = crc16ccitt(((char*)packet.getData() + 1), (packet.getSize() - 1));

    // crc
    packet.append(_CRC_);
    packet.append(RC_LABEL_SEP); // =
    packet.appendNumber(crcVerified);
    packet.append(RC_RECORD_SEP); // <CR>

    packet.append(RC_END_MSG); // <GS>

    return true;
}

bool Parser::makeFormatByteArrayFromRecordSet(QByteArray &outputData, const RecordSet &recordSet)
{
    outputData.clear();

    for (int i = 0; i < recordSet.getSize(); i++)
    {
        const Record& record = recordSet.getAt(i);

        outputData.append(record.getKeyByteArray());
        outputData.append(RC_LABEL_SEP); // =
        outputData.append(record.getValueByteArray());
        outputData.append(RC_RECORD_SEP); // <CR>
    }

    return true;
}

bool Parser::verifyPacketCrc(Packet& packet)
{
    if (packet.getPacketType() != Packet::PT_DATA ||
        packet.getCrcPosition() == 0)
    {
        packet.setCrcStatus(Packet::CS_NONE);
        return true;
    }

    if (packet.getSize() == 0)
    {
        packet.setCrcStatus(Packet::CS_ERROR);
        return false;
    }

    //                    *--- firstCrcTextPos = 10
    // 0   12345678   9   0123456789   0
    // <FS>REQ=INI<CR><RS>CRC=13402<CR><GS>
    // 1   23456789   0   1234567890   1    - len: 21
    //                             *--- lastCrcTextPos = 19

    int firstCrcTextPos = packet.getCrcPosition() + 1;
    int lastCrcTextPos = packet.getSize() - 2;

    if (firstCrcTextPos == lastCrcTextPos)
    {
        packet.setCrcStatus(Packet::CS_NONE);
        return true;
    }
    else if (firstCrcTextPos > lastCrcTextPos)
    {
        // really?
        packet.setCrcStatus(Packet::CS_ERROR);
        return false;
    }

    // calc crc
    int crcVerified = crc16ccitt(((char*)packet.getData() + 1), packet.getCrcPosition());

    // read crc
    Record crcRecord;

    if (parseRecord(packet.getData() + firstCrcTextPos, (lastCrcTextPos - firstCrcTextPos), crcRecord) == false)
    {
        packet.setCrcStatus(Packet::CS_ERROR);
        return false;
    }

    if (crcRecord.getValueSize() == 0)
    {
        packet.setCrcStatus(Packet::CS_NONE);
        return true;
    }

    // verify crc
    if (crcVerified != crcRecord.getValueAsInt())
    {
        packet.setCrcStatus(Packet::CS_ERROR);
        return false;
    }

    packet.setCrcStatus(Packet::CS_OK);
    return true;
}

void Parser::decodeEscape(const char* pData, int size, QByteArray& outputData)
{
    outputData.clear();

    if (pData == 0 || size <= 0)
        return;

    bool escape = false;
    char c;

    for (int i = 0; i < size; i++)
    {
        c = pData[i];

        if (escape)
        {
            c &= ~((char)(0x80));
            escape = false;
        }
        else if (c == RC_ESC)
        {
            escape = true;
            continue;
        }

        outputData.append(c);
    }

    return;
}

void Parser::splitRecordValues(const Record& record, QStringList& values)
{
    values = record.getValueAsString().split(RC_FIELD_SEP);
}

// ASCII absolute format - TRCFMT = 1;*;*;*;*
void Parser::splitRecordValues_AsciiAbsFormat(const Record& record, QList<int>& values, bool clearFlag)
{
    QStringList strValues;
    splitRecordValues(record, strValues);

    int value;
    bool ok;

    if (clearFlag)
        values.clear();

    for (int i = 0; i < strValues.size(); i++)
    {
        value = strValues.at(i).toInt(&ok);
        if (ok)
            values.append(value);
        else
            break;
    }

    return;
}

// Binary absolute format - TRCFMT = 2;*;*;*;*
void Parser::splitRecordValues_BinAbsFormat(const Record& record, QList<int>& values, bool clearFlag)
{
    QByteArray buffer;
    decodeEscape(record.getValueData(), record.getValueSize(), buffer);

    int value;
    qint16 data;
    char cc[2];

    if (clearFlag)
        values.clear();

    for (int i = 0; (i+1) < buffer.size(); i += sizeof(qint16))
    {
        cc[0] = buffer.at(i);
        cc[1] = buffer.at(i+1);

        memcpy(&data, cc, sizeof(qint16));
        value = data;

        values.append(value);
    }

    return;
}

// Binary differential format - TRCFMT = 3;*;*;*;*
void Parser::splitRecordValues_BinDiffFormat(const Record& record, QList<int>& values, bool clearFlag)
{
    QByteArray buffer;
    decodeEscape(record.getValueData(), record.getValueSize(), buffer);

    bool absoluteFlag = true;
    int value;
    qint16 data;
    char cc[2];
    char c;
    int pos = 0;

    if (clearFlag)
        values.clear();

    while (pos < buffer.size())
    {
        if (absoluteFlag)
        {
            if ((pos+1) >= buffer.size())
            {
                break;
            }
            else
            {
                cc[0] = buffer.at(pos);
                cc[1] = buffer.at(pos+1);

                memcpy(&data, cc, sizeof(qint16));
                value = data;
            }
            pos += sizeof(qint16);
            absoluteFlag = false;
        }
        else
        {
            c = buffer.at(pos);
            pos += sizeof(char);

            if (c == ((char)(0x80)))
            {
                absoluteFlag = true;
                continue;
            }

            value += (signed char)(c);
        }

        values.append(value);
    }

    return;
}

// Packed binary format - TRCFMT = 4;*;*;*;*
void Parser::splitRecordValues_PackedBinFormat(const Record& record, QList<int>& values, bool clearFlag)
{
    // TODO: implement
}

// Huvitz internal format 5 - TRCFMT = 5;*;*;*;*
void Parser::splitRecordValues_Huvitz5Format(const Record& record, QList<int>& values, bool clearFlag)
{
    QStringList strValues;
    splitRecordValues(record, strValues);

    int value;
    bool ok;

    if (clearFlag)
        values.clear();

    if (strValues.isEmpty())
        return;

    value = strValues.at(0).toInt(&ok);
    if (ok)
    {
        values.append(value);

        for (int i = 1; i < strValues.size(); i++)
        {
            value -= strValues.at(i).toInt(&ok);
            if (ok)
                values.append(value);
            else
                break;
        }
    }

    return;
}

// Huvitz internal format 6 - TRCFMT = 6;*;*;*;*
void Parser::splitRecordValues_Huvitz6Format(const Record& record, QList<int>& values, bool clearFlag)
{
    QStringList strValues;
    splitRecordValues(record, strValues);

    int value;
    bool ok;

    if (clearFlag)
        values.clear();

    if (strValues.isEmpty())
        return;

    value = strValues.at(0).toInt(&ok);
    if (ok)
    {
        values.append(value);

        for (int i = 1; i < strValues.size(); i++)
        {
            value += strValues.at(i).toInt(&ok);
            if (ok)
                values.append(value);
            else
                break;
        }
    }

    return;
}



void Parser::addRecordsToSet_AsciiAbsFormat(const QList<int>& values, RecordSet& recordSet, const QString& key)
{
    int i;
    QString value;
    bool first = true;

    for (i = 0; i < values.size(); i++)
    {
        if (first)
        {
            value += QString::number(values.at(i));
            first = false;
        }
        else
        {
            value += RC_FIELD_SEP;
            value += QString::number(values.at(i));
        }

        if (value.size() > 69)
        {
            recordSet.append(Record(key, value));
            value.clear();
            first = true;
        }
    }

    if (value.size() > 0)
        recordSet.append(Record(key, value));
}

void Parser::addRecordsToSet_BinAbsFormat(const QList<int>& values, RecordSet& recordSet, const QString& key)
{
    // TODO: implement
}

void Parser::addRecordsToSet_BinDiffFormat(const QList<int>& values, RecordSet& recordSet, const QString& key)
{
    // TODO: implement
}

void Parser::addRecordsToSet_PackedBinFormat(const QList<int>& values, RecordSet& recordSet, const QString& key)
{
    // TODO: implement
}

void Parser::addRecordsToSet_Huvitz5Format(const QList<int>& values, RecordSet& recordSet, const QString& key)
{
    int i;
    QString value;
    bool first = true;
    int prevValue = 0;

    for (i = 0; i < values.size(); i++)
    {
        if (first)
        {
            value += QString::number(values.at(i));
            first = false;
        }
        else
        {
            value += RC_FIELD_SEP;
            value += QString::number(prevValue - values.at(i));
        }

        prevValue = values.at(i);

        if (value.size() > 68)
        {
            recordSet.append(Record(key, value));
            value.clear();
            first = true;
        }
    }

    if (value.size() > 0)
        recordSet.append(Record(key, value));
}

void Parser::addRecordsToSet_Huvitz6Format(const QList<int>& values, RecordSet& recordSet, const QString& key)
{
    int i;
    QString value;
    bool first = true;
    int prevValue = 0;

    for (i = 0; i < values.size(); i++)
    {
        if (first)
        {
            value += QString::number(values.at(i));
        }
        else
        {
            value += RC_FIELD_SEP;
            value += QString::number(values.at(i) - prevValue);
        }

        prevValue = values.at(i);

        if (value.size() > 68)
        {
            recordSet.append(Record(key, value));
            value.clear();
        }
    }

    if (value.size() > 0)
        recordSet.append(Record(key, value));
}
