#include "mainwindow.h"
#include <QApplication>

#include "sdobserver.h"
#include "sdobserver2.h"
#include "sdobserver3.h"

int main(int argc, char *argv[])
{
#if defined(__arm__)
	QApplication a(argc, argv, QApplication::GuiServer);
#else
	QApplication a(argc, argv);
#endif

	MainWindow w;
	w.show();


	SdObserver2::instance().startObserver();
	SdObserver3::instance().startObserver();

	return a.exec();
}
