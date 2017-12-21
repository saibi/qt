#include <QApplication>
#ifdef __arm__
#include <QWSServer>
#endif

#include "imageviewer.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv, QApplication::GuiServer);

	ImageViewer imageviewer;
	imageviewer.showFullScreen();

	return a.exec();
}
