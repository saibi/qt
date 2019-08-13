#include "testjsonserializer.h"

#include "dummyserializable.h"

#include <QString>

const QString FILENAME = "test.json";
const QString DUMMY_FILE_CONTENT = "{\n    \"myBool\": true,\n    \"myDouble\": 5.2,\n    \"myInt\": 1,\n    \"myString\": \"hello\"\n}\n";

TestJsonSerializer::TestJsonSerializer(QObject *parent) :
	QObject(parent),
	mSerializer()
{

}

void TestJsonSerializer::saveDummy()
{
	DummySerializable dummy;

	dummy.myInt = 1;
	dummy.myDouble = 5.2;
	dummy.myString = "hello";
	dummy.myBool = true;

	mSerializer.save(dummy, FILENAME);

	QString data = loadFileContent();
	QVERIFY(data == DUMMY_FILE_CONTENT);
}


QString TestJsonSerializer::loadFileContent()
{
	QFile file(FILENAME);

	file.open(QFile::ReadOnly);

	QString content = file.readAll();
	file.close();
	return content;
}


void TestJsonSerializer::loadDummy()
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

void TestJsonSerializer::cleanup()
{
	QFile(FILENAME).remove();
}

//QTEST_MAIN(TestJsonSerializer)
