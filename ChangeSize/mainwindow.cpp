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

void MainWindow::on_pushButton_test_clicked()
{
	QSize size = ui->widget_test->size();

	qDebug("[%s] widget_test size = %d, %d", Q_FUNC_INFO, size.width(), size.height());

	if ( size.width() < 100 )
		ui->widget_test->resize(100, 100);
	else
		ui->widget_test->resize(50, 50);

	update();
}
