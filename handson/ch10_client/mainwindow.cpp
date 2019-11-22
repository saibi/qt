#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connectedToHost = false;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
	if ( !connectedToHost )
	{
		socket = new QTcpSocket();

		connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
		connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
		connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

		socket->connectToHost(ui->lineEdit_address->text(), 8001);
	}
	else
	{
		QString name = ui->lineEdit_name->text();
		socket->write("<font color='Orange'>" + name.toUtf8() + " has left the chat room.</font>");

		socket->disconnectFromHost();
	}
}

void MainWindow::socketConnected()
{
	qDebug() << "Connected to server.";

	printMessage("<font color='green'>Connected to server.</font>");

	QString name = ui->lineEdit_name->text();
	socket->write("<font color='purple'>" + name.toUtf8() + " has joined the chat room.</font>");

	ui->pushButton_connect->setText("Disconnect");
	connectedToHost = true;
}

void MainWindow::socketDisconnected()
{
	qDebug() << "Disconnected from server.";

	printMessage("<font color='red'>Disconnected from server.</font>");

	ui->pushButton_connect->setText("Connect");
	connectedToHost = false;
}

void MainWindow::socketReadyRead()
{
	ui->chatDisplay->append(socket->readAll());
}

void MainWindow::printMessage(QString message)
{
	ui->chatDisplay->append(message);
}

void MainWindow::on_pushButton_send_clicked()
{
	QString name = ui->lineEdit_name->text();
	QString message = ui->lineEdit_msg->text();

	socket->write("<font color='blue'>" + name.toUtf8() + "</font>: " + message.toUtf8());

	ui->lineEdit_msg->clear();
}
