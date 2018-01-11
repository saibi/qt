#include <QtGui>

#include "mainwindow.h"
#include "cachemodel.h"


cacheModelClass * cacheModel() {
	static cacheModelClass * p = new cacheModelClass();
	return p;
}



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setupUi(this);

	tableView->setModel(cacheModel());

	int nRowHeight = tableView->rowHeight(0);
	int nTableHeight = ( cacheModel()->rowCount() * nRowHeight) + tableView->horizontalHeader()->height() + 2 * tableView->frameWidth();

	tableView->setMinimumHeight(nTableHeight);
	tableView->setMaximumHeight(nTableHeight);

	int nColWidth = tableView->columnWidth(0);
	int nTableWidth = ( cacheModel()->columnCount() * nColWidth) + tableView->verticalHeader()->width() + 2 * tableView->frameWidth();
	
	tableView->setMinimumWidth(nTableWidth);
	tableView->setMaximumWidth(nTableWidth);

	connect(pushButton, SIGNAL( clicked() ), this, SLOT(getDataSetCache()));
}


void MainWindow::getDataSetCache() {
	int row = lineEdit->text().toInt();
	int col = lineEdit_2->text().toInt();
	int data = lineEdit_3->text().toInt();

	cacheModel()->setItem(row, col, data);
}
