#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
#ifdef Q_WS_X11
	qDebug("Q_WS_X11");
#elif Q_WS_QWS
	qDebug("Q_WS_QWS");
#elif Q_WS_WIN32
	qDebug("Q_WS_WIN32");
#else
	qDebug("Q_WS_ xxx");
#endif
}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::on_pushButton_hello_clicked()
{
	qDebug("[Dialog::on_pushButton_hello_clicked]");
	QMessageBox::information(this, tr("Hello World"), "Hello World Qt5", QMessageBox::Ok);
}

void Dialog::on_pushButton_close_clicked()
{
	qDebug("[Dialog::on_pushButton_close_clicked]");
	reject();
}
