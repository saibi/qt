#ifndef XMLSERIALIZER_H
#define XMLSERIALIZER_H

#include "serializer.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class XmlSerializer : public Serializer
{
public:
	XmlSerializer();

	void save(const Serializable & serializable, const QString & filePath, const QString & rootName) override;
	void load(Serializable & serializable, const QString & filePath) override;

private:
	void writeVariantToStream(const QString & nodeName, const QVariant& variant, QXmlStreamWriter& stream);
	void writeVariantValueToStream(const QVariant & variant, QXmlStreamWriter & stream);
	void writeVariantListToStream(const QVariant & variant, QXmlStreamWriter & stream);
	void writeVariantMapToStream(const QVariant & variant, QXmlStreamWriter & stream);

	QVariant readVariantFromStream(QXmlStreamReader & stream);
	QVariant readVariantValueFromStream(QXmlStreamReader & stream);
	QVariant readVariantListFromStream(QXmlStreamReader & stream);
	QVariant readVariantMapFromStream(QXmlStreamReader & stream);

};

#endif // XMLSERIALIZER_H
