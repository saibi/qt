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
		{
			readAdjRules();

			for (int i = 0 ; i < m_ruleList.size(); ++i )
				dumpAdjRule(m_ruleList.at(i));
		}
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
			m_rule.clear();
			m_rule.id = m_xml.attributes().value("id").toString().trimmed();
			m_rule.title = m_xml.attributes().value("title").toString().trimmed();

			QString pre = m_xml.attributes().value("prerequisite").toString().trimmed();
			if ( !pre.isEmpty() )
			{
				m_rule.prerequisite = pre.split(",");
				for ( int i = 0; i < m_rule.prerequisite.size(); ++i )
					m_rule.prerequisite[i] = m_rule.prerequisite[i].trimmed();
			}
			readRule();
			if ( !m_rule.isEmpty() )
				m_ruleList.append(m_rule);
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
			m_rule.desc = m_xml.readElementText().trimmed();
		else if ( m_xml.name() == "job" )
		{
			QString typeStr = m_xml.attributes().value("type").toString().trimmed();
			m_rule.jobType = (typeStr == "internal") ? AR_JOB_INTERNAL : (typeStr == "file") ? AR_JOB_FILE : AR_NONE;
			m_rule.job = m_xml.readElementText().trimmed();
		}
		else if ( m_xml.name() == "job_option" )
			m_rule.jobOptions = m_xml.readElementText();
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
			m_rule.dp.rotate = m_xml.readElementText().toFloat();
		else if ( m_xml.name() == "size" )
			m_rule.dp.size = m_xml.readElementText().toFloat();
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
			AR_InputData data;

			data.id = m_xml.attributes().value("id").toString().trimmed();
			QString typeStr = m_xml.attributes().value("type").toString().trimmed();
			data.type = typeStr == "float" ? AR_INPUT_FLOAT : typeStr == "int" ? AR_INPUT_INT : AR_NONE;
			data.min = m_xml.attributes().value("min").toString().toFloat();
			data.max = m_xml.attributes().value("max").toString().toFloat();
			data.step = m_xml.attributes().value("step").toString().toFloat();
			data.defaultValue = m_xml.attributes().value("default").toString().toFloat();

			if ( !data.isEmpty() )
				m_rule.inputList.append(data);

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
			AR_ApplyMethod method;

			method.src = m_xml.attributes().value("src").toString().trimmed();
			method.target = m_xml.attributes().value("target").toString().trimmed();
			QString opStr = m_xml.attributes().value("op").toString().trimmed();
			method.op = opStr == "set" ? AR_OP_SET : opStr == "add" ? AR_OP_ADD : AR_NONE;

			if ( !method.isEmpty() )
				m_rule.applyList.append(method);

			m_xml.skipCurrentElement();
		}
		else
			m_xml.skipCurrentElement();
	}
}

void EsaXmlReader::dumpAdjRule(const AdjRule &rule)
{
	if ( rule.isEmpty() )
	{
		qDebug("invalid rule");
		return;
	}
	qDebug("* rule id [%s], title [%s]", qPrintable(rule.id), qPrintable(rule.title));

	for ( int i = 0 ; i < rule.prerequisite.size(); ++i )
		qDebug("  prerequisite : %s", qPrintable(rule.prerequisite.at(i)));

	qDebug("desc [%s]", qPrintable(rule.desc));

	qDebug("job [%s] type-%d", qPrintable(rule.job), rule.jobType);

	if ( !rule.jobOptions.isEmpty() )
		qDebug("==========\n%s\n==========", qPrintable(rule.jobOptions));

	if ( !rule.dp.isEmpty() )
		qDebug("dp rotate %f, size %f", rule.dp.rotate, rule.dp.size);

	for ( int i = 0 ; i < rule.inputList.size(); ++i )
	{
		const AR_InputData & data = rule.inputList.at(i);
		if ( !data.isEmpty() )
			qDebug("input #%d id [%s] type [%d] min %f, max %f, default %f, step %f",
				   i, qPrintable(data.id), data.type, data.min, data.max, data.defaultValue, data.step );
	}

	for ( int i = 0 ; i < rule.applyList.size(); ++i )
	{
		const AR_ApplyMethod & method = rule.applyList.at(i);
		if ( !method.isEmpty() )
			qDebug("apply #%d %s -> %s : %d", i, qPrintable(method.src), qPrintable(method.target), method.op );
	}
}
