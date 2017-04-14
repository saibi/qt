#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPolygonF>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent *)
{
	QPainter p(this);

	QPolygonF frame;

	frame.append(QPointF(0.0, 0.0));
	frame.append(QPointF(0.0, 50.0));
	frame.append(QPointF(50.0, 50.0));
	frame.append(QPointF(50.0, 0.0));
	frame.append(QPointF(30.0, 0.0));
	frame.append(QPointF(30.0, 10.0));
	frame.append(QPointF(20.0, 10.0));
	frame.append(QPointF(20.0, 0.0));
	frame.append(QPointF(10.0, 0.0));

	qDebug("contains 30.0 30.0 = %d", frame.containsPoint(QPointF(30.0, 30.0), Qt::WindingFill));

	qDebug("contains 25.0 0.0 = %d", frame.containsPoint(QPointF(25.0, 0.0), Qt::WindingFill));






	frame.translate(QPointF(100, 100.0));
	p.setPen(QPen(Qt::red, 1.0));
	p.setBrush(Qt::NoBrush);

	p.drawPolygon(frame, Qt::WindingFill);

	p.setPen(QPen(Qt::blue, 2.0));
	p.setBrush(QBrush(Qt::blue));
	p.drawPoint(QPointF(130.0, 130.0));

	p.setPen(QPen(Qt::green, 2.0));
	p.setBrush(QBrush(Qt::green));
	p.drawPoint(QPointF(125.0, 100.0));

}


MainWindow::~MainWindow()
{
	delete ui;
}
