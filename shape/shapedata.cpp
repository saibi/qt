#include "shapedata.h"

ShapeData::ShapeData()
{
    clear();
}

ShapeData::~ShapeData()
{
    clear();
}

ShapeData::ShapeData(const ShapeData &data)
{
    set(data);
}

ShapeData & ShapeData::operator=(const ShapeData &data)
{
    set(data);
    return *this;
}

void ShapeData::clear()
{
    id = 0;
    R.clear();
    A.clear();
}

void ShapeData::set(const ShapeData &data)
{
    id = data.id;
    R = data.R;
    A = data.A;
}
