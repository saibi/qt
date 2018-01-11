#include <QtGui>
#include "test.h"


//#include "ticker.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Test *dlg = new Test;
	dlg->show();

#if 0
	Ticker *ticker = new Ticker;

	ticker->setText("123456789 ");
	ticker->show();
#endif

#if 0
	QWidget *window = new QWidget;
	Ticker *ticker = new Ticker;
	ticker->setText("hahaha hohoho hihihi");

	window->setWindowTitle("test timer event");

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(ticker);

	window->setLayout(layout);
	window->show();
#endif 
	return app.exec();
}
