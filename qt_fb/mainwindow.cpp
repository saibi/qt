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
	m_so.load(":/img/so.jpg");

	ui->label_1->setPixmap(m_aoa);
	ui->label_2->setPixmap(m_apink);


	ui->label_so->resize(m_so.width(), m_so.height());
	ui->label_so->setPixmap(m_so);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	static int idx = 0;


	QPoint pos = ui->widget_fb->mapToGlobal(QPoint(0, 0));

	qDebug("[%s] (%d, %d)", Q_FUNC_INFO, pos.x(), pos.y());
	FrameBuffer::instance().test(pos.x(), pos.y());


	if ( idx == 0 )
		FrameBuffer::instance().drawImg(pos.x() + 100, pos.y(), m_so.toImage());
	else if ( idx == 1)
		FrameBuffer::instance().drawImg(50, 50, m_apink.toImage());
	else if ( idx == 2 )
		FrameBuffer::instance().drawImg(pos.x(), 300, m_aoa.toImage());


	idx = (idx + 1) % 3;
}

void MainWindow::on_pushButton_exit_clicked()
{
	close();
}

void MainWindow::on_pushButton_clear_clicked()
{
	update();
}

void MainWindow::on_pushButton_buffer_clicked()
{
	int idx = FrameBuffer::instance().toggleBuffer();
	qDebug("[%s] %d", Q_FUNC_INFO, idx);
}

void MainWindow::on_pushButton_copy_clicked()
{
	FrameBuffer::instance().copy(0);
}
