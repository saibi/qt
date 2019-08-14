#include "testjsonserializer.h"
#include "testxmlserializer.h"
#include "testtrack.h"
#include "testgui.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QStringList arguments = QCoreApplication::arguments();

	int status = 0;
	TestJsonSerializer testJson;
	TestXmlSerializer testXml;
	TestTrack testTrack;
	TestGui testGui;

	status |= QTest::qExec(&testJson, arguments);
	status |= QTest::qExec(&testXml, arguments);
	status |= QTest::qExec(&testTrack, arguments);
	status |= QTest::qExec(&testGui, arguments);

	return status;
}
