#ifndef RECORDSET_H
#define RECORDSET_H

#include <QList>
#include <QMultiMap>
#include <QMutex>
#include "record.h"

class RecordSet
{
public:
    RecordSet();
    RecordSet(const RecordSet& recordSet);

    ~RecordSet();

    RecordSet& operator=(const RecordSet& recordSet);

    int append(const Record& record);

    void reset();

    int getSize() const;

    const Record& getAt(int idx) const {return _recordList.at(idx);}
    const Record& getAt(const QString& key) const;

    bool getValueAsStringByKey(const QString& key, QString& value, int index = -1) const;

    int getValueCountByKey(const QString& key) const;

protected:
    QList<Record> _recordList;

    QMultiMap<QString, int> _recordMap;

    mutable QMutex _mutex;

    Record _tmpRecord;
};

#endif // RECORDSET_H
