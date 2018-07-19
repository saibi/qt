#include "widget.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
	QTime t;
	t.start();

	qDebug("DBG before qapplication %d", t.elapsed());

	QApplication a(argc, argv);

	qDebug("DBG before widget %d", t.elapsed());

	Widget w;
	qDebug("DBG before show %d", t.elapsed());

	w.show();

	qDebug("DBG before a.exec %d", t.elapsed());

	return a.exec();
}
