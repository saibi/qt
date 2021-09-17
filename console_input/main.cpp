#include <QCoreApplication>

#include "inputthread.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);


	InputThread input;

	input.start();


	return a.exec();
}
