#ifndef ESAXMLREADER_H
#define ESAXMLREADER_H

#include <QXmlStreamReader>
#include "adjrule.h"

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


	void dumpAdjRule(const AdjRule & rule);

	QXmlStreamReader m_xml;

	AdjRule m_rule;
	QList <AdjRule> m_ruleList;
};

#endif // ESAXMLREADER_H
