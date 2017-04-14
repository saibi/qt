#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);


	QList <float> test;

	test.append(1.0);
	test.append(2.0);
	test.append(3.0);

	QList <float> target;

	target = test;

	target += 0.1;

	for (int i = 0 ; i < target.size(); i++ )
	{
		qDebug("%d %.2f", i, target.at(i));
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}
