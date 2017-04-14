#include "record.h"

Record::Record()
{
}

Record::Record(const Record& record)
{
    _key = record._key;
    _value = record._value;
}

Record::Record(const char* pKeyData, const char* pValueData, int valueSize)
{
    set(pKeyData, pValueData, valueSize);
}

Record::Record(const char* pKeyData, const QString& value)
{
    set(pKeyData, value);
}

Record::Record(const char* pKeyData, int value)
{
    set(pKeyData, value);
}

Record::Record(const char* pKeyData, unsigned int value)
{
    set(pKeyData, value);
}

Record::Record(const char* pKeyData, double value, int precision)
{
    set(pKeyData, value, precision);
}

Record::Record(const QString& key, const QString& value)
{
    set(key, value);
}

Record::Record(const QString& key, int value)
{
    set(key, value);
}

Record::Record(const QString& key, unsigned int value)
{
    set(key, value);
}

Record::Record(const QString& key, double value, int precision)
{
    set(key, value, precision);
}

Record::~Record()
{
}

Record& Record::operator=(const Record& record)
{
    _key = record._key;
    _value = record._value;
    return *this;
}
