#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framebuffer.h"

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

void MainWindow::on_pushButton_fb_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	FrameBuffer::instance().test();

}

void MainWindow::on_pushButton_img_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	QPixmap img(":/img/run.jpg");

	FrameBuffer::instance().drawImg( this->mapToGlobal(QPoint(100, 100)), img.toImage() );
}
