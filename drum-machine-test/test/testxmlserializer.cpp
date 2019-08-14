#include "testxmlserializer.h"
#include "dummyserializable.h"
#include <QDebug>

const QString FILENAME = "test.xml";
const QString DUMMY_FILE_CONTENT = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<dummy type=\"QVariantMap\">\n    <myBool type=\"bool\">true</myBool>\n    <myDouble type=\"double\">5.2</myDouble>\n    <myInt type=\"int\">1</myInt>\n    <myString type=\"QString\">hello</myString></dummy>\n";

TestXmlSerializer::TestXmlSerializer(QObject * parent) :
	QObject(parent),
	mSerializer()
{

}


void TestXmlSerializer::cleanup()
{
	QFile(FILENAME).remove();
}

void TestXmlSerializer::saveDummy()
{
	DummySerializable dummy;

	dummy.myInt = 1;
	dummy.myDouble = 5.2;
	dummy.myString = "hello";
	dummy.myBool = true;

	mSerializer.save(dummy, FILENAME, "dummy");

	QString data = loadFileContents();

	QVERIFY(data == DUMMY_FILE_CONTENT);
}

void TestXmlSerializer::loadDummy()
{
	QFile file(FILENAME);

	file.open(QFile::WriteOnly | QIODevice::Text);
	QTextStream out(&file);
	out << DUMMY_FILE_CONTENT;
	file.close();


	DummySerializable dummy;
	mSerializer.load(dummy, FILENAME);

	QCOMPARE(dummy.myInt, 1);
	QCOMPARE(dummy.myDouble, 5.2);
	QCOMPARE(dummy.myString, QString("hello"));
	QCOMPARE(dummy.myBool, true);
}

QString TestXmlSerializer::loadFileContents()
{
	QFile file(FILENAME);

	file.open(QFile::ReadOnly);

	QString content = file.readAll();
	file.close();
	return content;
}
