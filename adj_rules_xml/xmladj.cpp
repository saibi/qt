#include "xmladj.h"

#include <QFile>
#include <QDebug>

XmlAdj::XmlAdj(const QString & filename) :
	m_fileName(filename)
{}

void XmlAdj::read()
{
	if ( m_fileName.isEmpty() )
	{
		qDebug("[%s] empty filename", Q_FUNC_INFO);
		return;
	}

	QFile xmlFile(m_fileName);
    xmlFile.open(QIODevice::ReadOnly);
	m_xml.setDevice(&xmlFile);

	if (m_xml.readNextStartElement() && m_xml.name() == "adj_rules")
	   parseAdjRules();

    // readNextStartElement() leaves the stream in
    // an invalid state at the end. A single readNext()
    // will advance us to EndDocument.
	if (m_xml.tokenType() == QXmlStreamReader::Invalid)
		m_xml.readNext();

	if (m_xml.hasError())
	{
		m_xml.raiseError();
        qDebug() << errorString();
    }

	if ( m_adjRules.size() > 0 )
	{
		for (int i = 0; i < m_adjRules.size(); ++i )
		{
			dumpRule(m_adjRules.at(i));
		}
	}
}

void XmlAdj::dumpRule(const RuleElements &rule)
{
	qDebug("#%s [%s] %s\nJOB (%d) : %s", qPrintable(rule.id), qPrintable(rule.name), qPrintable(rule.desc), rule.jobType, qPrintable(rule.job));
}

void XmlAdj::parseAdjRules()
{
	if (!m_xml.isStartElement() || m_xml.name() != "adj_rules")
        return;

	while (m_xml.readNextStartElement())
	{
		if (m_xml.name() == "rule")
			parseRule();
        else
			m_xml.skipCurrentElement();
	}
}

// Uncomment this to see another way to read element
// text. It returns the concatenation of the text
// from all child elements.
//#define USE_READ_ELEMENT_TEXT 1

void XmlAdj::parseRule()
{
	if (!m_xml.isStartElement() || m_xml.name() != "rule")
        return;

	struct RuleElements rule;

	if ( m_xml.attributes().hasAttribute("id") )
	{
		rule.id = m_xml.attributes().value("id").toString();
	}

	while (m_xml.readNextStartElement())
	{
		if ( m_xml.name() == "name" )
		{
			rule.name = m_xml.readElementText();
		}
		else if ( m_xml.name() == "desc" )
		{
			rule.desc = m_xml.readElementText();
		}
		else if ( m_xml.name() == "job" )
		{
			QString typeStr = m_xml.attributes().value("type").toString();
			if ( typeStr == "adj" )
				rule.jobType = JOB_TYPE_ADJ;
			else if (typeStr == "sd" )
				rule.jobType = JOB_TYPE_SD;
			else
				rule.jobType = JOB_TYPE_INTERNAL_DATA;

			rule.job = m_xml.readElementText();
		}
	}
	m_adjRules.append(rule);
}

QString XmlAdj::errorString()
{
    return QObject::tr("%1\nLine %2, column %3")
			.arg(m_xml.errorString())
			.arg(m_xml.lineNumber())
			.arg(m_xml.columnNumber());
}
