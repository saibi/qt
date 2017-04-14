#include <QCoreApplication>
#include "cmdproc.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if ( argc > 1 )
	{
		CmdProc::procCmdLines(QCoreApplication::arguments());
		return 0;
	}
	else
	{
		CmdProc::usage();
		return 1;
	}

	return a.exec();
}
