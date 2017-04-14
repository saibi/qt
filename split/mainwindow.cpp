#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{

	qDebug("DBG lineEdit = [%s]", qPrintable(ui->lineEdit->text()));

	QStringList strList = ui->lineEdit->text().split(",", QString::SkipEmptyParts);

	qDebug("DBG size = %d", strList.size());

	for (int i = 0; i < strList.size(); ++i )
		qDebug("DBG #%d [%s]", i, qPrintable(strList.at(i)));

	qDebug("DBG first [%s], last [%s]", qPrintable(strList.first()), qPrintable(strList.last()) );

}
