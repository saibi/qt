#include "mainwindow.h"
#include <QApplication>

#if defined(__arm__)
#include <QWSServer>
#endif

int main(int argc, char *argv[])
{
#if defined(__arm__)
	QApplication a(argc, argv, QApplication::GuiServer);
	QWSServer::setCursorVisible(false);
	usleep(100*1000);
	QWSServer::setBackground(QBrush(Qt::black));
#else
	QApplication a(argc, argv);
#endif
	MainWindow w;
	w.show();

	return a.exec();
}


