#include "mainwindow.h"
#include <QApplication>

#include "dirimagewidget.h"

int main(int argc, char *argv[])
{
#ifdef __arm__
	QApplication a(argc, argv, QApplication::GuiServer);
	a.setOverrideCursor(Qt::BlankCursor);
#else
	QApplication a(argc, argv);
#endif

	QString imgPath = "/home/saibi/Pictures";

	if ( a.arguments().size() > 1 )
		imgPath = a.arguments().at(1);

	DirImageWidget i(imgPath);
	i.showFullScreen();

	return a.exec();
}
