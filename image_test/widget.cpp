#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
	setAttribute(Qt::WA_TranslucentBackground);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::showEvent(QShowEvent *)
{
	ui->widget_dirbtn_first->move(ui->widget_bottombg_2->pos());

	startTimer(60 * 1000);
}

void Widget::timerEvent(QTimerEvent *)
{
	ui->widget_dirbtn->move(ui->widget_bottombg->pos());

}

void Widget::on_pushButton_clicked()
{
	close();
}

#define STEP 10

void Widget::on_pushButton_left_clicked()
{
	ui->widget_dirbtn->move( ui->widget_dirbtn->x() - STEP, ui->widget_dirbtn->y());
}

void Widget::on_pushButton_right_clicked()
{
	ui->widget_dirbtn->move( ui->widget_dirbtn->x() + STEP, ui->widget_dirbtn->y());
}

void Widget::on_pushButton_down_clicked()
{
	ui->widget_dirbtn->move( ui->widget_dirbtn->x(), ui->widget_dirbtn->y() + STEP);
}

void Widget::on_pushButton_up_clicked()
{
	ui->widget_dirbtn->move( ui->widget_dirbtn->x(), ui->widget_dirbtn->y() - STEP);
}

void Widget::on_pushButton_left_bg_clicked()
{
	ui->widget_bottombg->move( ui->widget_bottombg->x() - STEP, ui->widget_bottombg->y());
}

void Widget::on_pushButton_right_bg_clicked()
{
	ui->widget_bottombg->move( ui->widget_bottombg->x() + STEP, ui->widget_bottombg->y());
}

void Widget::on_pushButton_down_bg_clicked()
{
	ui->widget_bottombg->move( ui->widget_bottombg->x(), ui->widget_bottombg->y() + STEP);
}

void Widget::on_pushButton_up_bg_clicked()
{
	ui->widget_bottombg->move( ui->widget_bottombg->x(), ui->widget_bottombg->y() - STEP);
}

void Widget::on_pushButton_merge_clicked()
{
	ui->widget_dirbtn->move(ui->widget_bottombg->pos());
}
