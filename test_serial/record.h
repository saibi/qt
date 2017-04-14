#ifndef RECORD_H
#define RECORD_H

#include <QByteArray>
#include <QString>

class Record
{
public:
    Record();
    Record(const Record& record);

    Record(const char* pKeyData, const char* pValueData, int valueSize = -1);
    Record(const char* pKeyData, const QString& value);
    Record(const char* pKeyData, int value);
    Record(const char* pKeyData, unsigned int value);
    Record(const char* pKeyData, double value, int precision = 2);
    Record(const QString& key, const QString& value);
    Record(const QString& key, int value);
    Record(const QString& key, unsigned int value);
    Record(const QString& key, double value, int precision = 2);

    ~Record();

    Record& operator=(const Record& record);

    void reset() {_key.clear(); _value.clear();}

    // Key
    const QByteArray& getKeyByteArray() const {return _key;}

    const char* getKeyData() const {return _key.constData();}

    int getKeySize() const {return _key.size();}

    void setKey(const char* pKeyData, int keySize = -1) {_key.clear(); _key.append(pKeyData, keySize);}
    void setKey(const QString& key) {_key = key.toUtf8();}

    QString getKeyAsString() const {return QString::fromUtf8(_key.constData(), _key.size());}

    // Value
    const QByteArray& getValueByteArray() const {return _value;}

    const char* getValueData() const {return _value.constData();}

    int getValueSize() const {return _value.size();}

    void setValue(const char* pValueData, int valueSize = -1) {_value.clear(); _value.append(pValueData, valueSize);}
    void setValue(const QString& value) {_value = value.toUtf8();}
    void setValue(int value) {setValue(QString::number(value));}
    void setValue(unsigned int value) {setValue(QString::number(value));}
    void setValue(double value, int precision = 2) {setValue(QString::number(value, 'f', precision));}

    QString getValueAsString() const {return QString::fromUtf8(_value.constData(), _value.size());}
    int getValueAsInt(bool* pOk = 0) const {return _value.toInt(pOk);}
    float getValueAsFloat(bool* pOk = 0) const {return _value.toFloat(pOk);}
    double getValueAsDouble(bool* pOk = 0) const {return _value.toDouble(pOk);}

    // Easy Access
    void set(const char* pKeyData, const char* pValueData, int valueSize = -1) {setKey(pKeyData); setValue(pValueData, valueSize);}
    void set(const char* pKeyData, const QString& value) {setKey(pKeyData); setValue(value);}
    void set(const char* pKeyData, int value) {setKey(pKeyData); setValue(value);}
    void set(const char* pKeyData, unsigned int value) {setKey(pKeyData); setValue(value);}
    void set(const char* pKeyData, double value, int precision = 2) {setKey(pKeyData); setValue(value, precision);}
    void set(const QString& key, const QString& value) {setKey(key); setValue(value);}
    void set(const QString& key, int value) {setKey(key); setValue(value);}
    void set(const QString& key, unsigned int value) {setKey(key); setValue(value);}
    void set(const QString& key, double value, int precision = 2) {setKey(key); setValue(value, precision);}

    QString getAsString() const {return QString::fromUtf8(_key.constData(), _key.size()) + '=' + QString::fromUtf8(_value.constData(), _value.size());}

protected:
    QByteArray _key;

    QByteArray _value;
};

#endif // RECORD_H
