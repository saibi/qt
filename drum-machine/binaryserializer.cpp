#include "binaryserializer.h"

#include <QFile>
#include <QDataStream>

BinarySerializer::BinarySerializer()
{

}

void BinarySerializer::save(const Serializable &serializable, const QString &filePath, const QString & /* rootName */)
{
	QFile file(filePath);
	file.open(QFile::WriteOnly);
	QDataStream dataStream(&file);
	dataStream << serializable.toVariant();
	file.close();
}


void BinarySerializer::load(Serializable &serializable, const QString &filePath)
{
	QFile file(filePath);
	file.open(QFile::ReadOnly);
	QDataStream dataStream(&file);
	QVariant variant;
	dataStream >> variant;
	serializable.fromVariant(variant);
	file.close();
}
