#include "widget.h"
#include <QApplication>

#if defined(__arm__)
#include <QWSServer>
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv, QApplication::GuiServer);
	Widget w;



#if defined(__arm__)
	QWSServer::setCursorVisible(false);
	QWSServer::setBackground(QBrush(Qt::black));

	w.showFullScreen();
#else
	w.show();
#endif
	return a.exec();
}
