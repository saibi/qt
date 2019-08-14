#ifndef TESTXMLSERIALIZER_H
#define TESTXMLSERIALIZER_H

#include <QtTest/QTest>
#include <xmlserializer.h>

class TestXmlSerializer : public QObject
{
	Q_OBJECT

public:
	TestXmlSerializer(QObject *parent = nullptr);

private slots:
	void cleanup();
	void saveDummy();
	void loadDummy();

private:
	QString loadFileContents();

private:
	XmlSerializer mSerializer;
};

#endif // TESTXMLSERIALIZER_H
