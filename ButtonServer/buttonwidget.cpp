#include "buttonwidget.h"
#include "ui_buttonwidget.h"

ButtonWidget::ButtonWidget(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ButtonWidget)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

ButtonWidget::~ButtonWidget()
{
	delete ui;
}

void ButtonWidget::on_pushButton_clicked()
{
	qDebug("[ButtonWidget::on_pushButton_clicked]");
}
