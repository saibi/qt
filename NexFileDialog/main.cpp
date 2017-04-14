#include "nexfiledialog.h"
#include <QApplication>

#include "util/sdobserver.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#if 1

	QString file_path;

//	SdObserver::instance().startObserver();

	qDebug("* Open File Test =======================");
	file_path = NexFileDialog::getOpenFileName(0, "Select File Test", "/tmp");
	qDebug("    RESULT - file_path = %s", qPrintable(file_path));

	qDebug("* Open File with Search Test ===========");
//	file_path = NexFileDialog::getOpenFileNameWithSearch(0, "OMA File Import", SdObserver::instance().getMountPath(), QStringList() << ".oma");
//	file_path = NexFileDialog::getOpenFileNameWithSearch(0, "OMA File Import", "/mnt/sdcard", QStringList() << ".oma");
	file_path = NexFileDialog::getOpenFileNameWithSearch(0, "OMA File Import", "/tmp");
	qDebug("    RESULT - file_path = %s", qPrintable(file_path));

	return 0;

#else

	NexFileDialog w;
	w.setTitle("OMA File Import");
	w.setWorkingDir("/mnt/sdcard");
//	w.setSearchRoot(SdObserver::instance().getMountPath() + "/oma");
	w.show();

	return a.exec();

#endif
}
