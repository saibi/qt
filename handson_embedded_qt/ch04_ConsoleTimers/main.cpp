#include "timecatcher.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	TimeCatcher tc;

	return a.exec();
}
