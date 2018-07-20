#include "esawidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ESAWidget w;
	w.show();

	return a.exec();
}
