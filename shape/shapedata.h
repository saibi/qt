#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include <QList>

class ShapeData
{    

public:
    ShapeData();
	virtual ~ShapeData();
    ShapeData(const ShapeData & data);

    ShapeData& operator=(const ShapeData & data);

    virtual void clear();

    void set(const ShapeData & data);

    bool isNull() { return (R.isEmpty() && A.isEmpty()); }
    bool isEmpty() { return isNull(); }

    int size() const { return R.size(); } // ASSERT R.size() == A.size()
    void importRA(const QList<float> & inR, const QList<float> & inA) { R = inR; A = inA; }

public:
    unsigned int id;

    QList <float> R;
    QList <float> A;

};

#endif // SHAPEDATA_H
