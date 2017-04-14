#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->installEventFilter(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{

	Dialog dlg(this);


	dlg.exec();
}
