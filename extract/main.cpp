#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Dialog w;
	w.show();

	//w.extractGuiBinaryBuildDate("/tmp/nex_wedge", 0);
	//w.extractGuiBinaryVersionStr("/tmp/nex_wedge", 0);

	w.readGuiBinaryBuildInfoManual("/tmp/nex_wedge");
	w.readGuiBinaryBuildInfoManual("/tmp/hpe910");

	VERSION_INFO ver;
	QStringList list;

	list << "/tmp/nex_wedge" << "/tmp/hpe910";

	for (int i = 0; i < list.size(); ++i )
	{
		if ( w.readGuiBinaryBuildInfo(list[i], ver) )
		{
			qDebug("DBG %s - %d.%d.%d %s", qPrintable(list[i]), ver.major, ver.minor, ver.patch, qPrintable(ver.date.toString("yyyy/M/d")));
		}
	}

	return a.exec();
}
