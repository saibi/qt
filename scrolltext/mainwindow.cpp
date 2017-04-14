#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->widget->setInterval(10);
	ui->widget->setText("This is ScrollText widget. from stackoverflow.com hohoho hahahaha");
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::showEvent(QShowEvent *)
{
}
