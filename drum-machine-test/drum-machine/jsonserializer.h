#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "serializer.h"

class JsonSerializer : public Serializer
{
public:
	JsonSerializer();

	void save(const Serializable & serializable, const QString & filePath, const QString & rootName = "") override;
	void load(Serializable & serializable, const QString & filePath) override;
};

#endif // JSONSERIALIZER_H
