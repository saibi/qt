#include "clientdialog.h"
#include "ui_clientdialog.h"

#include <QMessageBox>

ClientDialog::ClientDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ClientDialog)
{
	ui->setupUi(this);

	m_socket = new QLocalSocket(this);

	if ( ! m_socket )
	{
		qDebug("create error");
		return;
	}

	connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(displayError(QLocalSocket::LocalSocketError)));
	connect(m_socket, SIGNAL(connected()), this, SLOT(slotConnected()));

	m_socket->connectToServer("TouchButtonServer", QIODevice::WriteOnly);


}

ClientDialog::~ClientDialog()
{
	delete ui;
}

void ClientDialog::displayError(QLocalSocket::LocalSocketError socketError)
{
	switch (socketError) {
	case QLocalSocket::ServerNotFoundError:
		QMessageBox::information(this, tr("Client"),
								 tr("The host was not found. Please check the "
									"host name and port settings."));
		break;

	case QLocalSocket::ConnectionRefusedError:
		QMessageBox::information(this, tr("Client"),
								 tr("The connection was refused by the peer. "
									"Make sure the server is running, "
									"and check that the host name and port "
									"settings are correct."));
		break;

	case QLocalSocket::PeerClosedError:
		break;

	default:
		QMessageBox::information(this, tr("Client"),
								 tr("The following error occurred: %1.")
								 .arg(m_socket->errorString()));
	}
}

void ClientDialog::slotConnected()
{
	qDebug("[ClientDialog::slotConnected]");
}

void ClientDialog::on_pushButton_clicked()
{
	if ( m_socket && m_socket->isWritable() )
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);

		out.setVersion(QDataStream::Qt_4_7);
		out << ui->lineEdit->text();

		m_socket->write(block);
		m_socket->flush();
		qDebug("[ClientDialog::on_pushButton_clicked] write [%s]", qPrintable(ui->lineEdit->text()));
	}
}
