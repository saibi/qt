#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fb.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_aoa.load(":/img/aoa.jpg");
	m_apink.load(":/img/apink.jpg");

	ui->label_1->setPixmap(m_aoa);
	ui->label_2->setPixmap(m_apink);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	QPoint pos = ui->widget_fb->mapToGlobal(QPoint(0, 0));

	qDebug("[%s] (%d, %d)", Q_FUNC_INFO, pos.x(), pos.y());

	fb_test(pos.x(), pos.y());
}

void MainWindow::on_pushButton_exit_clicked()
{
	close();
}
