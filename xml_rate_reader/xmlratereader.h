#ifndef XMLRATEREADER_H
#define XMLRATEREADER_H

#include <QXmlStreamReader>
#include <QString>

class XmlRateReader
{
public:
    XmlRateReader(const QString filename);

    void read();

private:
    void processRates();
    void processRate();
    QString readNextText();
    QString errorString();

    QString _filename;
    QXmlStreamReader xml;
};

#endif // XMLRATEREADER_H
