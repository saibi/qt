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

void MainWindow::paintEvent(QPaintEvent *)
{
#if 0
	QPainter painter(this);

	painter.setBrush(QBrush(Qt::transparent, Qt::SolidPattern));
	painter.setPen(QPen(Qt::black, 2.0, Qt::SolidLine));

	painter.drawLine(QPoint(0.0, 0.0), this->rect().bottomRight());
#endif

}

void MainWindow::on_pushButton_inc_clicked()
{
	ui->widget_canvas->resize( ui->widget_canvas->width() + 10, ui->widget_canvas->height() + 10);

}

void MainWindow::on_pushButton_dec_clicked()
{
	ui->widget_canvas->resize( ui->widget_canvas->width() - 10, ui->widget_canvas->height() - 10);

}

void MainWindow::on_pushButton_left_clicked()
{
	ui->widget_canvas->move( ui->widget_canvas->x() - 5, ui->widget_canvas->y());
}

void MainWindow::on_pushButton_right_clicked()
{
	ui->widget_canvas->move( ui->widget_canvas->x() + 5, ui->widget_canvas->y());
}
