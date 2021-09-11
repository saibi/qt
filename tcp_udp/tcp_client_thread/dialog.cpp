#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

	m_socketThread = 0;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_test_clicked()
{
	qDebug("[UI] [Dialog::on_pushButton_test_clicked]");

	if ( !m_socketThread )
	{
		m_socketThread = new TcpSocketThread(QHostAddress("127.0.0.1"), 5244, this);

		Q_CHECK_PTR(m_socketThread);

		m_socketThread->start();
	}
}
