#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QString>

#include "serializable.h"

class Serializer
{
public:
	virtual ~Serializer() {}
	virtual void save(const Serializable& serializable, const QString& filePath, const QString& rootName = "") = 0;
	virtual void load(Serializable & serializable, const QString& filePath) = 0;
};

#endif // SERIALIZER_H
