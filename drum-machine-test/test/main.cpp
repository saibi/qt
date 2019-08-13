#include "testjsonserializer.h"
#include "testxmlserializer.h"
#include "testtrack.h"

int main(int argc, char *argv[])
{
	int status = 0;
	TestJsonSerializer testJson;
	TestXmlSerializer testXml;
	TestTrack testTrack;
	QStringList arguments = QCoreApplication::arguments();

	status |= QTest::qExec(&testJson, arguments);
	status |= QTest::qExec(&testXml, arguments);
	status |= QTest::qExec(&testTrack, arguments);

	return status;
}
