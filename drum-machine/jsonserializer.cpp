#include "jsonserializer.h"

#include <QJsonDocument>
#include <QFile>

JsonSerializer::JsonSerializer()
{

}

void JsonSerializer::save(const Serializable& serializable, const QString& filePath, const QString & /*rootName*/)
{
	QJsonDocument doc = QJsonDocument::fromVariant(serializable.toVariant());
	QFile file(filePath);
	file.open(QFile::WriteOnly);
	file.write(doc.toJson());
	file.close();
}

void JsonSerializer::load(Serializable & serializable, const QString& filePath)
{
	QFile file(filePath);
	file.open(QFile::ReadOnly);
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	file.close();
	serializable.fromVariant(doc.toVariant());
}
