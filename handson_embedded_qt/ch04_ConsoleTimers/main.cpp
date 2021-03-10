#include "timecatcher.h"

#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>

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


	QVariant v;
	v.setValue(10);

	int y = 41 * v.toInt();
	qDebug() << v.toString() << " * 41 = " << y;
	qDebug() << "v as a bool = " << v.toBool();
	qDebug() << "v as a list = " << v.toList();


	QList<QVariant> l;
	l << v;
	l << QVariant(1);
	l << QVariant(2.3);
	l << QVariant("Hello World");

	qDebug() << "The list: " << l;

	return a.exec();
}
