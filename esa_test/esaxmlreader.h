#ifndef ESAXMLREADER_H
#define ESAXMLREADER_H

#include <QXmlStreamReader>
#include "adjrule.h"

class EsaXmlReader
{
public:
	EsaXmlReader();

	int read(const QString & path, QList <AdjRule> & ruleList);

private:
	QString errorString() const;

	void readAdjRules(QList <AdjRule> & ruleList);
	void readRule();
	void readDp();
	void readInputList();
	void readApplyList();

	QXmlStreamReader m_xml;
	AdjRule m_rule;
};

#endif // ESAXMLREADER_H
