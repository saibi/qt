#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>

#include "tcpsocketthread.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->groupBox_device->setEnabled(false);

	m_udpSocket = new QUdpSocket(this);
	connect(m_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams );

	m_tcpServer = new TcpServer(this);

	if ( !m_tcpServer->listen(QHostAddress::Any, 8279) )
	{
		qDebug() << "listen() error : " << m_tcpServer->errorString();
		m_tcpServer->close();
	}
	connect(m_tcpServer, &TcpServer::signalConnected, this, &MainWindow::slot_clientConnected);
	connect(m_tcpServer, &TcpServer::signalDisconnected, this, &MainWindow::slot_clientDisconnected);
	connect(ui->widget_discoverList, &DiscoverListForm::signalDeviceSelected, this, &MainWindow::slot_clientSelected);
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
	QByteArray data = datagram.data().trimmed();

	qDebug() << datagram.senderAddress() << datagram.senderPort();
	qDebug() << datagram.data();

	if ( data.startsWith("ew ready") )
	{
		QString id(data);
		id = id.replace("ew ready", "").trimmed();
		ui->widget_discoverList->discoverUpdate(QDateTime::currentDateTime(), id, QHostAddress(datagram.senderAddress().toIPv4Address()).toString());
	}
}

void MainWindow::on_pushButton_broadcast_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_broadcast_clicked]");

	ui->groupBox_device->setEnabled(false);
	ui->label_ip->clear();
	ui->widget_discoverList->clear();

	QByteArray datagram = "ew hello";
	m_udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 8279);
}

void MainWindow::slot_clientSelected(const QString & id, const QString & ip)
{
	m_clientId = id;
	m_clientIp = ip;

	ui->label_ip->setText(ip);
	ui->groupBox_device->setEnabled(true);
}

void MainWindow::slot_clientConnected(TcpSocketThread *thread)
{
	m_thread = thread;
	ui->groupBox_device->setEnabled(false);
	m_thread->setTransfer(ui->lineEdit_transfer->text().toInt());
	m_thread->setRepeat(ui->lineEdit_repeat->text().toInt());
	m_thread->start();
}

void MainWindow::slot_clientDisconnected()
{
	m_thread = nullptr;
	ui->groupBox_device->setEnabled(true);
}


void MainWindow::on_pushButton_connect_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_connect_clicked]");

	QByteArray datagram = "ew con 8279 " + m_clientId.toLocal8Bit();

	m_udpSocket->writeDatagram(datagram, QHostAddress(m_clientIp), 8279);
}

void MainWindow::on_pushButton_delay_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_delay_clicked]");

	QByteArray datagram = QString("ew delay " + ui->lineEdit_delay->text()).toLocal8Bit();

	m_udpSocket->writeDatagram(datagram, QHostAddress(m_clientIp), 8279);
}

