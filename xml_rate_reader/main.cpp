#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

#include "xmlratereader.h"

void readRates(const QString filename) {
    // Three of the different ways we can parse XML in Qt.
    // All are equivalent, Dom and XQuery are much smaller,
    // with Dom being easier to understand, but Xquery being
    // much more powerful in fewer lines.

	XmlRateReader reader(filename);
	reader.read();

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    switch(args.size()) {
    default:
		qDebug() << "Usage:" << qPrintable(args[0]) << "rates.xml";
        return 1;
	case 2:
        break;
    }

    readRates(args[1]);
    return 0;
}
