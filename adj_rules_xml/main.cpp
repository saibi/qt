#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

#include "xmladj.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    switch(args.size()) {
    default:
		qDebug() << "Usage:" << qPrintable(args[0]) << "adj.xml";
        return 1;
	case 2:
        break;
    }

	XmlAdj reader(args[1]);
	reader.read();

	return 0;
}
