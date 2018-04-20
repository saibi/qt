#ifndef XMLADJ_H
#define XMLADJ_H

#include <QXmlStreamReader>
#include <QString>

struct RuleElements
{
	QString id;
	QString name;
	QString desc;
	QString job;
	int jobType;
};

enum JobTypeIds
{
	JOB_TYPE_INTERNAL_DATA = 0,
	JOB_TYPE_ADJ,
	JOB_TYPE_SD,
};

class XmlAdj
{
public:
	XmlAdj(const QString & filename);

	void setFilename(const QString & filename) { m_fileName = filename; }
    void read();

private:
	void parseAdjRules();
	void parseRule();
    QString errorString();

	void dumpRule(const struct RuleElements & rule);


	QString m_fileName;
	QXmlStreamReader m_xml;

	QList < struct RuleElements > m_adjRules;
};

#endif // XMLADJ_H
