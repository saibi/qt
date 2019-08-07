#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	mWorker(),
	mWorkerThread(this)
{
	ui->setupUi(this);

	connect(ui->pushButton_stop, &QPushButton::clicked, this, &MainWindow::handleStop);
	mWorker.moveToThread(&mWorkerThread);

	mWorkerThread.start();
}

MainWindow::~MainWindow()
{
	delete ui;

	handleStop();

}


void MainWindow::handleStop()
{
	mWorkerThread.quit();
	mWorkerThread.wait(1000);
}

void MainWindow::handleStart()
{
	mWorkerThread.start();
}
