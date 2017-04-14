#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drillshapedata.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);


	DrillShapeData a;

	qDebug("Angle = %f, R = %f, A = %f", a.angle, a.R.at(0), a.A.at(0));

	a.R.append(1.0);
	a.A.append(3.0);
	a.angle = 18.8;

	qDebug("Angle = %f, R = %f, A = %f", a.angle, a.R.at(0), a.A.at(0));

	DrillShapeData b;

	qDebug("Angle = %f, R = %f, A = %f", b.angle, b.R.at(0), b.A.at(0));

	b = a;

	qDebug("Angle = %f, R = %f, A = %f", b.angle, b.R.at(0), b.A.at(0));

}

MainWindow::~MainWindow()
{
	delete ui;
}
