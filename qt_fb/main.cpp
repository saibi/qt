#include "mainwindow.h"
#include <QApplication>
#ifdef __arm__
#include <QWSServer>
#endif


int main(int argc, char *argv[])
{
#ifdef __arm__
	QApplication a(argc, argv, QApplication::GuiServer);
#else
	QApplication a(argc, argv);
#endif
	MainWindow w;

#ifdef __arm__
	w.showFullScreen();
#else
	w.show();
#endif

	return a.exec();
}
