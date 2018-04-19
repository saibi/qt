#include "xmladj.h"

#include <QFile>
#include <QDebug>

XmlAdj::XmlAdj(const QString & filename) :
    _filename(filename)
{}

void XmlAdj::read() {
    QFile xmlFile(_filename);
    xmlFile.open(QIODevice::ReadOnly);
    xml.setDevice(&xmlFile);

	if (xml.readNextStartElement() && xml.name() == "adj_rules")
	   processAdjRules();

    // readNextStartElement() leaves the stream in
    // an invalid state at the end. A single readNext()
    // will advance us to EndDocument.
    if (xml.tokenType() == QXmlStreamReader::Invalid)
        xml.readNext();

	if (xml.hasError())
	{
        xml.raiseError();
        qDebug() << errorString();
    }
}

void XmlAdj::processAdjRules() {
	if (!xml.isStartElement() || xml.name() != "adj_rules")
        return;
	while (xml.readNextStartElement())
	{
		if (xml.name() == "rule")
			processRule();
        else
            xml.skipCurrentElement();
	}
}

// Uncomment this to see another way to read element
// text. It returns the concatenation of the text
// from all child elements.
//#define USE_READ_ELEMENT_TEXT 1

void XmlAdj::processRule()
{
	if (!xml.isStartElement() || xml.name() != "rule")
        return;

	if ( xml.attributes().hasAttribute("id") )
		qDebug("id = %s", qPrintable(xml.attributes().value("id").toString()));

	while (xml.readNextStartElement())
	{

		if ( xml.name() == "job" && xml.isStartElement() )
		{

			qDebug("#%d", xml.attributes().size());

			QString n;
			if ( xml.attributes().hasAttribute("type") )
				n = xml.attributes().value("type").toString();

			qDebug("[%s %s] = %s", qPrintable(xml.name().toString()), qPrintable(n), qPrintable(xml.readElementText()));

			//qDebug("%s = %s", qPrintable(xml.name().toString()), qPrintable(xml.attributes().value("type").toString()), qPrintable(xml.readElementText()));
			//if ( xml.attributes().hasAttribute("test") )
			//	qDebug("type attr");
		}
		else
			qDebug("[%s] = %s", qPrintable(xml.name().toString()), qPrintable(xml.readElementText()));


    }
}

QString XmlAdj::errorString()
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}
