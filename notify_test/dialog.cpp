#include "dialog.h"
#include "ui_dialog.h"

#include "notification.h"
#include "noti.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
}

Dialog::~Dialog()
{
	delete ui;
	qDebug("Dialog::~Dialog()");
}

void Dialog::on_pushButton_clicked()
{
	qDebug("[Dialog::on_pushButton_clicked]");

	Notification::instance().notify("TEST");

}

void Dialog::on_pushButton_test_clicked()
{
	qDebug("[Dialog::on_pushButton_test_clicked]");
	noti::instance().notify("test");
}
