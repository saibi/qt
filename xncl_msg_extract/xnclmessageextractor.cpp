#include "xnclmessageextractor.h"

#include <QFile>
#include <QFileInfo>

#include <QDebug>

XnclMessageExtractor::XnclMessageExtractor()
{
	m_checkTr = false;
}

bool XnclMessageExtractor::loadXML(const QString &filePath)
{
	QFile file(filePath);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		qDebug("[XnclMessageExtractor::loadXML] Error: file open [%s]",
			   filePath.toLocal8Bit().constData());
		return false;
	}

	m_message_list.clear();
	bool ret = loadXML(&file);

//	if(ret && m_message_list.count() > 0)
//	{
//		saveMessageTable(filePath);
//	}

	return ret;
}

bool XnclMessageExtractor::loadXML(QIODevice *device)
{
	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!m_domDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn))
	{
		qDebug("[XnclMessageExtractor::loadXML] Error: setContent - Line: [%d], Column: [%d], Message: [%s]",
			   errorLine, errorColumn, errorStr.toLocal8Bit().constData());
		return false;
	}

	QDomElement root = m_domDocument.documentElement();

	if (root.tagName() != "xncl")
	{
		qDebug("[XnclMessageExtractor::loadXML] Error: Not XNCL file - Tag: [%s]",
			   root.tagName().toLocal8Bit().constData());
		return false;
	}

	//
	QString version;

	if (!root.hasAttribute("version"))
	{
		qDebug("[XnclMessageExtractor::loadXML] Error: No version string");
		return false;
	}

	version = root.attribute("version");

	if (version != "1.0")
	{
		qDebug("[XnclMessageExtractor::loadXML] Error: Invalid version - Version: [%s]",
			   version.toLocal8Bit().constData());
		return false;
	}

	//
	QString section;

	if (!root.hasAttribute("section"))
	{
		qDebug("[XnclMessageExtractor::loadXML] Error: No section string");
		return false;
	}

	section = root.attribute("section").trimmed();

	if (section.isEmpty())
	{
		qDebug("[XnclMessageExtractor::loadXML] Error: Empty section string");
		return false;
	}

	//
	qDebug("[XnclMessageExtractor::loadXML] Version: [%s], Section: [%s]",
		   version.toLocal8Bit().constData(),
		   section.toLocal8Bit().constData());

	m_section = section;

	// group
	QDomElement childGroup = root.firstChildElement("group");
	while (!childGroup.isNull())
	{
		parseGroupElement(childGroup, 1);
		childGroup = childGroup.nextSiblingElement("group");
	}

	//
	qDebug("[XnclMessageExtractor::loadXML] OK");

	return true;
}

void XnclMessageExtractor::parseGroupElement(const QDomElement &element, int depth)
{
	QString tag;

	QDomElement child = element.firstChildElement();
	while (!child.isNull())
	{
		tag = child.tagName();

		if (tag == "item")
		{
			parseItemElement(child, depth);
		}
		else if (tag == "group")
		{
			if ( m_checkTr  == false || child.attribute("tr").simplified().toInt() )
			{
				addMessage(child.attribute("text"));
			}

			// child
			parseGroupElement(child, depth+1);
		}
		else
		{
			qDebug("[XnclMessageExtractor::parseGroupElement] Ignore: unknown tag - Tag: [%s]", tag.toLocal8Bit().constData());
		}

		child = child.nextSiblingElement();
	}
}

void XnclMessageExtractor::parseItemElement(const QDomElement &element, int depth)
{
	if (!element.hasAttribute("type"))
	{
		qDebug("[XnclMessageExtractor::parseItemElement] Error: The item does not have type.");
		return;
	}

	QString type = element.attribute("type");

	if (  m_checkTr  == false || element.attribute("tr").simplified().toInt() )
	{
		addMessage(element.attribute("text"));
	}

	if (type.compare("INPUT") == 0)
	{
		if (element.hasAttribute("default"))
		{
			addMessage(element.attribute("default"));
		}
	}
	else if (type.compare("STATIC") == 0)
	{
	}
	else if (type.compare("STATIC_DATA") == 0)
	{
	}
	else if (type.compare("CHECK") == 0)
	{
	}
	else if (type.compare("RADIO") == 0)
	{
		QDomElement child = element.firstChildElement("choice");
		while (!child.isNull())
		{
			parseRadioElement(child);
			child = child.nextSiblingElement("choice");
		}
	}
	else if (type.compare("COMBO") == 0)
	{
		QDomElement child = element.firstChildElement("choice");
		while (!child.isNull())
		{
			parseComboElement(child);
			child = child.nextSiblingElement("choice");
		}
	}
	else if (type.compare("SEPARATOR") == 0)
	{
	}
	else
	{
		// unknown type
		qDebug("[XnclMessageExtractor::parseItemElement] Error: The item has unknown type - Type: [%s]",
			   type.toLocal8Bit().constData());
	}

	return;
}

void XnclMessageExtractor::parseRadioElement(const QDomElement &element)
{
	if (!element.hasAttribute("text") || !element.hasAttribute("data"))
	{
		qDebug("[XnclMessageExtractor::parseRadioElement] Error: The radio item does not have text or data.");
		return;
	}

	if (  m_checkTr  == false || element.attribute("tr").simplified().toInt())
	{
		addMessage(element.attribute("text"));
	}
}

void XnclMessageExtractor::parseComboElement(const QDomElement &element)
{
	if (!element.hasAttribute("text") || !element.hasAttribute("data"))
	{
		qDebug("[XnclMessageExtractor::parseComboElement] Error: The combo item does not have text or data.");
		return;
	}

	if ( m_checkTr  == false || element.attribute("tr").simplified().toInt())
	{
		addMessage(element.attribute("text"));
	}
}

void XnclMessageExtractor::addMessage(QString message)
{
	if(m_message_list.contains(message) == false)
	{
		bool ok;
		message.toDouble(&ok);
		if(!ok)
		{
			m_message_list << message;
		}
	}
}

void XnclMessageExtractor::saveMessageTable(QString file_path)
{
	QFileInfo	file_info(file_path);
	QString		out_file_path = file_info.baseName() + ".msg";

	QFile out_file(out_file_path);

	if (!out_file.open(QFile::WriteOnly | QFile::Text))
	{
		qDebug("[XnclMessageExtractor::loadXML] Error: file open [%s]",
			   out_file_path.toLocal8Bit().constData());
		return;
	}

	QString data;
	for(int i = 0 ; i < m_message_list.count() ; i++)
	{
		data = "tr(\"" + m_message_list.at(i) + "\")\n";
		out_file.write(data.toLocal8Bit());
	}

	out_file.close();
}

QStringList XnclMessageExtractor::getMessageList(void)
{
	return m_message_list;
}
