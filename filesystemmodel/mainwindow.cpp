#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_dirModel = new QFileSystemModel(this);
	m_dirModel->setRootPath("/home/saibi");
	m_dirModel->setFilter(QDir::AllDirs | QDir::Files);
	ui->treeView->setModel(m_dirModel);
	ui->treeView->setRootIndex( m_dirModel->index("/home/saibi"));
}

MainWindow::~MainWindow()
{
	delete ui;
}
