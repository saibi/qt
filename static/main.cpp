#include "dialog.h"
#include <QApplication>

#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//Dialog w;
	//w.show();

	static QLabel label; // static cause SIGSEGV. why???

	label.setText("TEST");
	label.show();

	return a.exec();
}
