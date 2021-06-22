#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_udpSocket = new QUdpSocket(this);

	connect(m_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams );
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::readPendingDatagrams()
{
	while (m_udpSocket->hasPendingDatagrams())
	{

		QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
		processDatagram(datagram);
	}
}

void MainWindow::processDatagram(QNetworkDatagram datagram)
{
	qDebug() << datagram.senderAddress() << datagram.senderPort();
	qDebug() << datagram.data();

	if ( datagram.data().compare("ew ready") == 0 )
	{
		ui->widget_discoverList->discoverUpdate(QDateTime::currentDateTime(), "hohoho", QHostAddress(datagram.senderAddress().toIPv4Address()).toString());
	}


}

void MainWindow::on_pushButton_broadcast_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_broadcast_clicked]");

	ui->widget_discoverList->clear();

	QByteArray datagram = "ew hello";
	m_udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 8279);
}
