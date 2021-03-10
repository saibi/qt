#include "timecatcher.h"

#include <QCoreApplication>
#include <QTextStream>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	TimeCatcher tc;

	QTextStream a_cout(stdout);
	QTimer tocTimer;
	tocTimer.setInterval(1000);
	tocTimer.setSingleShot(false);

	QObject::connect(&tocTimer, &QTimer::timeout, [&a_cout] {
		a_cout << "tic " << Qt::endl;
	});
	tocTimer.start();


	return a.exec();
}
