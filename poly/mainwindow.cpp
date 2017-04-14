#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_dlg = new Dialog(this);
	m_dlg->setText("hohohoho");

	m_dlg2 = new Dialog2(this);
	m_dlg2->setText("hihihi");

}

MainWindow::~MainWindow()
{
	delete ui;

	delete m_dlg;
	delete m_dlg2;
}

void MainWindow::on_pushButton_run_clicked()
{

	if ( m_dlg )
		m_dlg->show();
}

void MainWindow::on_pushButton_run2_clicked()
{
	if ( m_dlg2 )
		m_dlg2->show();
}
