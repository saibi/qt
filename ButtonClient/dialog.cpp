#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);

	socket = new QLocalSocket(this);


//    connect(hostLineEdit, SIGNAL(textChanged(QString)),
//            this, SLOT(enableGetFortuneButton()));
//    connect(getFortuneButton, SIGNAL(clicked()),
//            this, SLOT(requestNewFortune()));
//    connect(socket, SIGNAL(readyRead()), this, SLOT(readFortune()));

	connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
			this, SLOT(displayError(QLocalSocket::LocalSocketError)));

	connect(socket, SIGNAL(connected()), this, SLOT(slotConnected()));

}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::on_pushButton_exit_clicked()
{
	this->accept();
}

void Dialog::displayError(QLocalSocket::LocalSocketError socketError)
{
	switch (socketError) {
	case QLocalSocket::ServerNotFoundError:
		QMessageBox::information(this, tr("Button Client"),
								 tr("The host was not found. Please check the "
									"host name and port settings."));
		break;

	case QLocalSocket::ConnectionRefusedError:
		QMessageBox::information(this, tr("Button Client"),
								 tr("The connection was refused by the peer. "
									"Make sure the button server is running, "
									"and check that the host name and port "
									"settings are correct."));
		break;

	case QLocalSocket::PeerClosedError:
		break;

	default:
		QMessageBox::information(this, tr("Button Client"),
								 tr("The following error occurred: %1.")
								 .arg(socket->errorString()));
	}

	ui->pushButton_send->setEnabled(true);
}

void Dialog::on_pushButton_send_clicked()
{
	qDebug("Dialog::on_pushButton_send_clicked()");

	sendCommand(ui->lineEdit_cmd->text());
	ui->pushButton_send->setEnabled(false);

	qDebug("Dialog::on_pushButton_send_clicked() end");

}

void Dialog::slotConnected()
{
	qDebug("Dialog::slotConnected()");
	ui->pushButton_send->setEnabled(true);

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);

	out.setVersion(QDataStream::Qt_4_7);
	out << m_cmdStr;

	socket->write(block);
	socket->flush();
	socket->disconnectFromServer();

	qDebug("Dialog::slotConnected() end");
}

void Dialog::sendCommand(const QString &cmdStr)
{
	m_cmdStr = cmdStr;
	socket->abort();
	socket->connectToServer("ButtonServer");
}

void Dialog::on_pushButton_btn1_clicked()
{
	qDebug("Dialog::on_pushButton_btn1_clicked()");
	sendCommand("id 0");
}

void Dialog::on_pushButton_btn2_clicked()
{
	qDebug("Dialog::on_pushButton_btn2_clicked()");
	sendCommand("id 1");
}

void Dialog::on_pushButton_show_clicked()
{
	qDebug("Dialog::on_pushButton_show_clicked()");
	sendCommand("show");
}

void Dialog::on_pushButton_hide_clicked()
{
	qDebug("Dialog::on_pushButton_hide_clicked()");
	sendCommand("hide");
}

void Dialog::on_pushButton_up_clicked()
{
	qDebug("Dialog::on_pushButton_up_clicked()");
	sendCommand("move up");
}

void Dialog::on_pushButton_down_clicked()
{
	qDebug("Dialog::on_pushButton_down_clicked()");
	sendCommand("move down");
}

void Dialog::on_pushButton_right_clicked()
{
	qDebug("Dialog::on_pushButton_right_clicked()");
	sendCommand("move right");
}

void Dialog::on_pushButton_left_clicked()
{
	qDebug("Dialog::on_pushButton_left_clicked()");
	sendCommand("move left");
}
