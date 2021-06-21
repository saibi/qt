#include "mainwindow.h"
#include "mocktempsensor.h"

#include <QApplication>

int main(int argc, char *argv[])
{

	qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
	qputenv("QT_LOGGING_RULES", QByteArray("qt.virtualkeyboard=true"));


	QApplication a(argc, argv);
	MockTempSensor mockTemp;
	MainWindow w(&mockTemp);
	w.show();
	return a.exec();
}
