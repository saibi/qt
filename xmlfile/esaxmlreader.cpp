#include "esaxmlreader.h"
#include <QFile>
#include <QDebug>

EsaXmlReader::EsaXmlReader()
{

}

bool EsaXmlReader::read(const QString & path)
{
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug("[EsaXmlReader::read] file open error %s", qPrintable(path));
		return false;
	}

	m_xml.setDevice(&file);

	if (m_xml.readNextStartElement()) {
		if (m_xml.name() == "adj_rules" && m_xml.attributes().value("version") == "0.1")
			readAdjRules();
		else
			m_xml.raiseError(QObject::tr("The file is not an ESA xml file."));
	}

	return !m_xml.error();
}

QString EsaXmlReader::errorString() const
{
	return QObject::tr("Line %1, column %2 : %3").arg(m_xml.lineNumber()).arg(m_xml.columnNumber()).arg(m_xml.errorString());
}

void EsaXmlReader::readAdjRules()
{
	while (m_xml.readNextStartElement())
	{
		if (m_xml.name() == "rule")
		{
			qDebug("# rule id %d, title [%s], pre %d", m_xml.attributes().value("id").toString().toInt(), qPrintable(m_xml.attributes().value("title").toString()) , m_xml.attributes().value("prerequisite").toString().toInt());
			readRule();
		}
		else
			m_xml.skipCurrentElement();

	}
	if ( m_xml.error() )
		qDebug("ERROR : %s", qPrintable(errorString()));
}

void EsaXmlReader::readRule()
{
	while (m_xml.readNextStartElement())
	{
		qDebug() << m_xml.lineNumber() << m_xml.name();

		if (m_xml.name() == "desc")
			qDebug("desc[%s]", qPrintable(m_xml.readElementText()));
		else if ( m_xml.name() == "job" )
		{
			qDebug("job [%s] type-%s", qPrintable(m_xml.readElementText()), qPrintable(m_xml.attributes().value("type").toString()));
		}
		else if ( m_xml.name() == "job_option" )
		{
			qDebug("==========\n%s\n==========", qPrintable(m_xml.readElementText()));
		}
		else if ( m_xml.name() == "dp" )
			readDp();
		else if ( m_xml.name() == "input_list" )
			readInputList();
		else if ( m_xml.name() == "apply_list" )
			readApplyList();
		else
			m_xml.skipCurrentElement();
	}
}


void EsaXmlReader::readDp()
{
	while (m_xml.readNextStartElement())
	{
		if ( m_xml.name() == "rotate" )
			qDebug("dp rotate %f", m_xml.readElementText().toFloat());
		else if ( m_xml.name() == "size" )
			qDebug("dp size %f", m_xml.readElementText().toFloat());
		else
			m_xml.skipCurrentElement();
	}
}

void EsaXmlReader::readInputList()
{
	while (m_xml.readNextStartElement())
	{
		if ( m_xml.name() == "input" )
		{
			qDebug("input id [%s] type [%s] min %f, max %f, default %f, step %f",
				   qPrintable(m_xml.attributes().value("id").toString()),
				   qPrintable(m_xml.attributes().value("type").toString()),
				   m_xml.attributes().value("min").toString().toFloat(),
				   m_xml.attributes().value("max").toString().toFloat(),
				   m_xml.attributes().value("default").toString().toFloat(),
				   m_xml.attributes().value("step").toString().toFloat() );
			m_xml.skipCurrentElement();
		}
		else
			m_xml.skipCurrentElement();
	}
}

void EsaXmlReader::readApplyList()
{
	while (m_xml.readNextStartElement())
	{
		if ( m_xml.name() == "apply" )
		{
			qDebug("apply %s -> %s : %s",
				   qPrintable(m_xml.attributes().value("src").toString()),
				   qPrintable(m_xml.attributes().value("target").toString()),
				   qPrintable(m_xml.attributes().value("op").toString()));
			m_xml.skipCurrentElement();
		}
		else
			m_xml.skipCurrentElement();
	}
}
