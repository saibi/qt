#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "myfiledialog.h"

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

void MainWindow::on_pushButton_clicked()
{

	//QFileDialog::getOpenFileName(this,"open","", "*.txt");

	MyFileDialog::getOpenFileName(this, "open test", "", "text files (*.txt);;all files (*.*)");
}
