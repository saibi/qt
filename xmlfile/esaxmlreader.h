#ifndef ESAXMLREADER_H
#define ESAXMLREADER_H

#include <QXmlStreamReader>

class EsaXmlReader
{
public:
	EsaXmlReader();

	bool read(const QString & path);

private:
	QString errorString() const;

	void readAdjRules();
	void readRule();
	void readDp();
	void readInputList();
	void readApplyList();


	QXmlStreamReader m_xml;

};

#endif // ESAXMLREADER_H
