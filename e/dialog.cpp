#include "dialog.h"
#include "ui_dialog.h"

#include "configmanager.h"

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

void Dialog::on_pushButton_test_clicked()
{
	qDebug("[UI] [Dialog::on_pushButton_test_clicked]");

	try {
		ConfigManager::instance().reopen();
		ConfigManager::instance().setData();
		ConfigManager::instance().close();
	}
	catch ( int error )
	{
		qDebug("catch error = %d", error);
	}
}
