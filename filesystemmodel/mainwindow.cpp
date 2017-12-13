#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_cwd = "/home/saibi";

	m_dirModel = new QFileSystemModel(this);
	m_dirModel->setRootPath("/");
	m_dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDot);
	ui->treeView->setModel(m_dirModel);
	ui->treeView->setRootIndex( m_dirModel->index(m_cwd));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
	qDebug("[%s] start", Q_FUNC_INFO);

	QFileInfo f = m_dirModel->fileInfo(index);

	if ( f.isDir() )
	{
		m_cwd = f.absoluteFilePath();
		qDebug("[%s] %s", Q_FUNC_INFO, qPrintable(f.absoluteFilePath()));

		ui->treeView->setRootIndex(m_dirModel->index(m_cwd) );
	}
}
