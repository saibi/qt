#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);

	//setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

	m_socket = 0;
	m_server = new QLocalServer(this);


	if ( !m_server->listen("TouchButtonServer") )
	{
		QMessageBox::critical(this, tr("LocalServer"), tr("Unable to start the server: %1").arg(m_server->errorString()));
		delete m_server;
		m_server = 0;
		return;
	}

	connect(m_server, SIGNAL(newConnection()), this, SLOT(slotConnected()));

	qDebug("server created");
}

Dialog::~Dialog()
{
	delete ui;

	if ( m_server )
		delete m_server;

	qDebug("server deleted");
}

void Dialog::slotConnected()
{
	qDebug("[Dialog::slotConnected()]");

	if ( m_socket )
	{
		m_socket->disconnectFromServer();
		m_socket = 0;
	}

	m_socket = m_server->nextPendingConnection();

	if ( m_socket )
	{
		connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadCommand()));
		connect(m_socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));

		qDebug("[Dialog::slotConnected()] next socket");
	}
}

void Dialog::slotReadCommand()
{
	if ( !m_socket )
		return;

	QDataStream in(m_socket);
	in.setVersion(QDataStream::Qt_4_7);

	QString cmd;

	in >> cmd;

	qDebug("[Dialog::slotReadCommand()] cmd = [%s]", qPrintable(cmd));

	emit signalCommandReceived(cmd);
}

void Dialog::on_pushButton_clicked()
{
	static int count = 0;

	qDebug("[Dialog::on_pushButton_clicked()]");

	++count;

	if ( count > 2 )
		close();

}
