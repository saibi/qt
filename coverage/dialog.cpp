#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::on_pushButton_1_clicked()
{
	qDebug("1 clicked");
}

void Dialog::on_pushButton_2_clicked()
{
	qDebug("2 clicked");
}
