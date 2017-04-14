#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "quickadj.h"
#include "dlg_checkbox.h"
#include "dlg_step.h"

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

void MainWindow::on_pushButton_quickAdj_clicked()
{
	QuickAdj dlg(this);

	dlg.exec();
}

void MainWindow::on_pushButton_quickAdj_2_clicked()
{
	Dlg_Checkbox dlg(this);

	dlg.exec();
}

void MainWindow::on_pushButton_quickAdj_3_clicked()
{
	Dlg_Step dlg(this);

	dlg.exec();
}
