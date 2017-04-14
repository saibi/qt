#include "recordset.h"

#include <QMutexLocker>

RecordSet::RecordSet()
 : _mutex(QMutex::Recursive)
{
}

RecordSet::RecordSet(const RecordSet& recordSet)
 : _mutex(QMutex::Recursive)
{
    _recordList = recordSet._recordList;
    _recordMap = recordSet._recordMap;
}

RecordSet::~RecordSet()
{
}

RecordSet& RecordSet::operator=(const RecordSet& recordSet)
{
    _recordList = recordSet._recordList;
    _recordMap = recordSet._recordMap;
    return *this;
}

int RecordSet::append(const Record& record)
{
    QMutexLocker locker(&_mutex);
    _recordList.append(record);
    _recordMap.insert(record.getKeyAsString(), _recordList.size()-1);
    return 0;
}

void RecordSet::reset()
{
    QMutexLocker locker(&_mutex);
    _recordList.clear();
    _recordMap.clear();
}

int RecordSet::getSize() const
{
    QMutexLocker locker(&_mutex);
    return _recordList.size();
}

const Record& RecordSet::getAt(const QString& key) const
{
    QMutexLocker locker(&_mutex);
    if (_recordMap.count(key) >= 1)
    {
        return _recordList.at(_recordMap.value(key));
    }
    return _tmpRecord;
}

bool RecordSet::getValueAsStringByKey(const QString& key, QString& value, int index) const
{
    QMutexLocker locker(&_mutex);

    int count = _recordMap.count(key);

    // QMap::values - Returns a list containing all the values associated with key key, from the most recently inserted to the least recently inserted one.
    if (index < 0)
        index = count - 1;

    if (count > 0 && index >= 0 && index < count)
    {
        QList<int> values = _recordMap.values(key);
        value = _recordList.at(values.at(index)).getValueAsString();
        return true;
    }

    value.clear();
    return false;
}

int RecordSet::getValueCountByKey(const QString& key) const
{
    QMutexLocker locker(&_mutex);
    return _recordMap.count(key);
}
