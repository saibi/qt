#include <QtGui>
#include "stringlistmodel.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QStringList numbers;

	numbers << "One" << "Two" << "Three" << "Four" << "Five";

	QAbstractItemModel *model = new StringListModel(numbers);


#if 0
	QListView *view = new QListView;
	view->setModel(model);

	view->show();
#endif 

	QTableView *firstTableView = new QTableView;
	QTableView *secondTableView = new QTableView;

	firstTableView->setModel(model);
	secondTableView->setModel(model);
	secondTableView->setSelectionModel(firstTableView->selectionModel());

	firstTableView->show();
	secondTableView->show();

	return app.exec();
}

