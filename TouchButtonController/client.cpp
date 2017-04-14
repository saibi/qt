#include "client.h"
#include "ui_client.h"
#include "controller.h"

#include <QTimer>

Client::Client(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Client)
{
	ui->setupUi(this);

	//Controller::instance().launchServer("/tmp/TouchButtonServer");

	QTimer::singleShot(1000, this, SLOT(slotConnect()));
}

Client::~Client()
{	
	delete ui;
	Controller::instance().disconnectFromServer();

}

void Client::on_pushButton_send_clicked()
{
	qDebug("[UI] [Client::on_pushButton_send_clicked]");
	Controller::instance().sendCommand(ui->lineEdit_cmd->text());
}

void Client::slotConnect()
{
	Controller::instance().connectToServer();
}

void Client::on_pushButton_id0_clicked()
{
	qDebug("[UI] [Client::on_pushButton_id0_clicked]");
	Controller::instance().sendCommand("id 0");
}

void Client::on_pushButton_id1_clicked()
{
	qDebug("[UI] [Client::on_pushButton_id1_clicked]");
	Controller::instance().sendCommand("id 1");
}

void Client::on_pushButton_up_clicked()
{
	qDebug("[UI] [Client::on_pushButton_up_clicked]");
	Controller::instance().sendCommand("move up");
}

void Client::on_pushButton_left_clicked()
{
	qDebug("[UI] [Client::on_pushButton_left_clicked]");
	Controller::instance().sendCommand("move left");
}

void Client::on_pushButton_right_clicked()
{
	qDebug("[UI] [Client::on_pushButton_right_clicked]");
	Controller::instance().sendCommand("move right");
}

void Client::on_pushButton_down_clicked()
{
	qDebug("[UI] [Client::on_pushButton_down_clicked]");
	Controller::instance().sendCommand("move down");
}

void Client::on_pushButton_show_clicked()
{
	qDebug("[UI] [Client::on_pushButton_show_clicked]");
	Controller::instance().sendCommand("show");
}

void Client::on_pushButton_hide_clicked()
{
	qDebug("[UI] [Client::on_pushButton_hide_clicked]");
	Controller::instance().sendCommand("hide");
}
