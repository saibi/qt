#include <QtGui>
#include "find.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	FindFileDialog f;

	f.show();

	return app.exec();
}
