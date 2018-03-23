#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	if ( argc > 1 )
	{
		w.dumpDb(argv[1]);
		return 0;
	}

	w.show();

	return a.exec();
}
