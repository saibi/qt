#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	scene = new QGraphicsScene(this);
	ui->graphicsView->setScene(scene);

	QBrush greenBrush(Qt::green);
	QBrush blueBrush(Qt::blue);
	QPen pen(Qt::black);
	pen.setWidth(2);

	QGraphicsRectItem * rectangle = scene->addRect(80, 0, 80, 80, pen, greenBrush);
	QGraphicsEllipseItem * ellipse = scene->addEllipse(0, -80, 200, 60, pen, blueBrush);
	QGraphicsTextItem * text = scene->addText("Hello World!", QFont("Times", 25));

}

MainWindow::~MainWindow()
{
	delete ui;
}
