#include <QtGui/QApplication>
#include "mainwindow.h"
#include "beep.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc > 1 )
	{
		if ( strcmp(argv[1], "-v") == 0 )
		{
			printf("V0.0.1\n");
			return 0;
		}
	}

	qDebug("language tester start");
#if defined(__arm__)
	Beep::beepInformation();
#endif

	QApplication a(argc, argv);
	MainWindow w;

#if defined(__arm__)
	w.showFullScreen();
#else
	w.show();
#endif

	return a.exec();
}
