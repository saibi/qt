#include <QtGui/QApplication>
#include "mainwindow.h"
#include "cmdproc.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	if ( argc > 1 )
	{
		CmdProc::procCmdLines(QCoreApplication::arguments());
		return 0;
	}

    MainWindow w;
    w.show();

    return a.exec();
}
