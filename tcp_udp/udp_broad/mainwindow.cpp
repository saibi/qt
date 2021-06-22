#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_udpSocket = new QUdpSocket(this);
	connect(m_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams );

	m_tcpServer = new QTcpServer(this);

	if ( !m_tcpServer->listen(QHostAddress::Any, 8279) )
	{
		qDebug() << "listen() error : " << m_tcpServer->errorString();
		m_tcpServer->close();
	}
	else
	{
		connect(m_tcpServer, &QTcpServer::newConnection, this, &MainWindow::handleNewConnection);
	}

	connect(ui->widget_discoverList, &DiscoverListForm::signalDeviceSelected, this, &MainWindow::sendReqConnect);

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

	ui->widget_discoverList->clear();

	QByteArray datagram = "ew hello";
	m_udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 8279);
}

void MainWindow::handleNewConnection()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_10);

	out << "tcp connected";

	QTcpSocket *clientConnection = m_tcpServer->nextPendingConnection();
	connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);

	clientConnection->write("tcp connected");
	clientConnection->disconnectFromHost();
}

void MainWindow::sendReqConnect(const QString & id, const QString & ip)
{
	QByteArray datagram = "ew con 8279 " + id.toLocal8Bit();

	m_udpSocket->writeDatagram(datagram, QHostAddress(ip), 8279);
}
