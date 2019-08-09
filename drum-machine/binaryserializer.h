#ifndef BINARYSERIALIZER_H
#define BINARYSERIALIZER_H

#include "serializer.h"

class BinarySerializer : public Serializer

{
public:
	BinarySerializer();

	void save(const Serializable & serializable, const QString & filePath, const QString & rootName) override;
	void load(Serializable & serializable, const QString & filePath) override;
};

#endif // BINARYSERIALIZER_H
