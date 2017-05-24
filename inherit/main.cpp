#include "childdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ChildDialog w;
	w.show();

	return a.exec();
}
