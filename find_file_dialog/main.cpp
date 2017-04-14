#include "dlg_findfiles.h"
#include <QApplication>

#include "util/sdobserver.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	SdObserver::instance().startObserver();

	Dlg_FindFiles w;
	w.setTitle("OMA File Import");
	w.setSearchRoot("/tmp");
//	w.setSearchRoot(SdObserver::instance().getMountPath() + "/oma");
	w.show();

	return a.exec();
}
