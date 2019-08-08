#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	if ( argc > 1 )
	{
		QString path = QString(argv[1]);
		QString md5 = MainWindow::getFileMD5( path );
		MainWindow::saveMD5( path + ".md5", md5);
		return 0;
	}
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
