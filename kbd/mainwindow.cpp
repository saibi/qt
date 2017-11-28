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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	qDebug("[key %d]", event->key());
	ui->label_result->setText(QString("[key %1]").arg(event->key()));

	if ( event->key() == Qt::Key_Escape )
		qApp->quit();
}
