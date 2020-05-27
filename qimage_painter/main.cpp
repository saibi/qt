#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if defined(__arm__) && defined(Q_WS_QWS)
	QApplication a(argc, argv, QApplication::GuiServer);
#else
	QApplication a(argc, argv);
#endif

	MainWindow w;
	w.show();

	return a.exec();
}
