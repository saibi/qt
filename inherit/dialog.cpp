#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);

	connect(ui->pushButton_hello, SIGNAL(clicked()), this, SLOT(slot_pushButton_hello_clicked()));
}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::slot_pushButton_hello_clicked()
{
	qDebug("[UI] [Dialog::slot_pushButton_hello_clicked]");
}
