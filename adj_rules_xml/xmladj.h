#ifndef XMLADJ_H
#define XMLADJ_H

#include <QXmlStreamReader>
#include <QString>

class XmlAdj
{
public:
	XmlAdj(const QString & filename);

    void read();

private:
	void processAdjRules();
	void processRule();
    QString readNextText();
    QString errorString();

    QString _filename;
    QXmlStreamReader xml;
};

#endif // XMLADJ_H
