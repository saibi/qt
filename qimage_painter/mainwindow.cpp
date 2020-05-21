#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

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


void MainWindow::showEvent(QShowEvent *event)
{
	QImage im(QSize(200, 200), QImage::Format_RGB32);

	im.fill(QColor(200, 10, 30).rgba());

	QPainter p;

	p.begin(&im);

	p.setPen(QPen(Qt::black, 5.0, Qt::SolidLine, Qt::RoundCap));

	p.drawEllipse(20, 20, 160, 160);

	p.drawArc(70, 115, 60, 30, 200 * 16, 140 * 16);

	p.setBrush(Qt::black);

	p.drawEllipse(65, 70, 15, 15);

	p.drawEllipse(120, 70, 15, 15);

	p.end();


	ui->label->setPixmap(QPixmap::fromImage(im));

	QMainWindow::showEvent(event);
}
