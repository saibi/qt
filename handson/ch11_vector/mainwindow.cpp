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

void MainWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	painter.begin(this);

	painter.drawLine(QPoint(50, 60), QPoint(100, 100));

	painter.setBrush(Qt::BDiagPattern);
	painter.drawRect(QRect(40, 120, 80, 30));

	QPen ellipsePen;
	ellipsePen.setColor(Qt::red);
	ellipsePen.setStyle(Qt::DashDotLine);
	painter.setPen(ellipsePen);
	painter.drawEllipse(QPoint(80, 200), 50, 20);

	QPainterPath rectPath;
	rectPath.addRect(QRect(150, 20, 100, 50));
	painter.setPen(QPen(Qt::red, 1, Qt::DashDotLine, Qt::FlatCap, Qt::MiterJoin));
	painter.setBrush(Qt::yellow);
	painter.drawPath(rectPath);


	QPainterPath ellipsePath;
	ellipsePath.addEllipse(QPoint(200, 120), 50, 20);
	painter.setPen(QPen(QColor(79, 106, 25), 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter.setBrush(QColor(122, 163, 39));
	painter.drawPath(ellipsePath);

	painter.end();
}
