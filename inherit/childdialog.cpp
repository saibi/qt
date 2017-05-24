#include "childdialog.h"
#include "ui_childdialog.h"
#include <QMessageBox>

ChildDialog::ChildDialog(QWidget *parent) :
	Dialog(parent),
	ui(new Ui::ChildDialog)
{
	ui->setupUi(this);
}

ChildDialog::~ChildDialog()
{
	delete ui;
}

void ChildDialog::slot_pushButton_hello_clicked()
{
	qDebug("[UI] [ChildDialog::slot_pushButton_hello_clicked]");

	QMessageBox::information(this, tr("Greetings"), tr("Hello World !!!") + "    \n\n\n", QMessageBox::Ok);

}
