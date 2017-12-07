#include <QApplication>

#include "imageviewer.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	ImageViewer imageviewer;
	imageviewer.show();

	return a.exec();
}
