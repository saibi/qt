#include "childdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ChildDialog w;

	w.showFullScreen();

	return a.exec();
}
