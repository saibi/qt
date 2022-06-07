#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Dialog w;
	w.show();

	w.extractGuiBinaryBuildDate("/tmp/nex_wedge", 0);
	w.extractGuiBinaryVersionStr("/tmp/nex_wedge", 0);
	return a.exec();
}
