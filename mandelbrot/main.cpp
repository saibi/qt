#include "mainwindow.h"
#include <QApplication>

#include <QVector>
#include "jobresult.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qRegisterMetaType<JobResult>("JobResult");
	qRegisterMetaType<QList<JobResult>>("QList<JobResult>");

	MainWindow w;
	w.show();

	return a.exec();
}
