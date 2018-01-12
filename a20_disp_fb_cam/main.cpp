#include "mainwindow.h"
#include <QApplication>
#ifdef __arm__
#include <QWSServer>
#endif
#include "framebuffer.h"

int main(int argc, char *argv[])
{
#ifdef __arm__
	QApplication a(argc, argv, QApplication::GuiServer);
#else
	QApplication a(argc, argv);
#endif
	MainWindow w;
	w.show();

	return a.exec();
}
