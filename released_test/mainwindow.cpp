#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

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

void MainWindow::showEvent(QShowEvent *)
{
	QTimer::singleShot(5*1000, this, SLOT(slot_hide()));
}

void MainWindow::on_pushButton_pressed()
{
	qDebug("pressed");

}

void MainWindow::on_pushButton_released()
{
	qDebug("released");
}

void MainWindow::slot_hide()
{
	ui->pushButton->hide();
}
