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
	ui->widget_tcp_test->setEnabled(false);

	m_udpSocket = new QUdpSocket(this);
	connect(m_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams );

	m_tcpServer = new TcpServer(this);

	if ( !m_tcpServer->listen(QHostAddress::Any) )
	{
		qDebug() << "listen() error : " << m_tcpServer->errorString();
		m_tcpServer->close();
	}

	qDebug() << "listen port :" << m_tcpServer->serverPort();

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
	m_udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, DEVICE_UDP_PORT);
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
	m_thread->start();
	qDebug() << "DBG clientConnected";

	ui->widget_tcp_test->setEnabled(true);
	ui->pushButton_connect->setText("connected");
}

void MainWindow::slot_clientDisconnected()
{
	m_thread = nullptr;
	qDebug() << "DBG clientDisConnected";

	ui->widget_tcp_test->setEnabled(false);
	ui->pushButton_connect->setText("Connect");

}


void MainWindow::on_pushButton_delay_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_delay_clicked]");

	QByteArray datagram = QString("ew delay " + ui->lineEdit_delay->text()).toLocal8Bit();

	m_udpSocket->writeDatagram(datagram, QHostAddress(m_clientIp), DEVICE_UDP_PORT);
}

void MainWindow::on_pushButton_test_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_test_clicked]");

	if ( !m_thread )
		return;

	static unsigned int counter = 0;

	int max = ui->lineEdit_repeat->text().toInt();
	int size = ui->lineEdit_transfer->text().toInt();

	for ( int i = 0 ; i < max ; ++ i )
	{
		QByteArray data;

		QString first = QString::asprintf("#%d;", counter++);
		first += QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");

		data.fill(0, size);
		data.prepend(first.toLocal8Bit());
		data.resize(size);

		TcpPacket3 packet;

		switch(i%11)
		{

		case 1:
			packet.set(TcpPacket3::FLAG_BIT_ENCRYPTION | TcpPacket3::FLAG_BIT_CHECKSUM, TcpPacket3::TYPE_NONE, data);
			break;

		case 2:
			packet.setCmdLine(TcpPacket3::FLAG_BIT_CHECKSUM, "set time " + QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
			break;

		case 3:
			packet.setSmallFile(TcpPacket3::FLAG_NONE, "hello.txt", "plain\nhello world\nhello world\n");
			break;

		case 4:
			packet.setSmallFile(TcpPacket3::FLAG_BIT_CHECKSUM, "hello2.txt", "checksum\nhello world\nhello world\n");
			break;

		case 5:
			packet.setSmallFile(TcpPacket3::FLAG_BIT_ENCRYPTION, "hello3.txt", "encryption\nhello world\nhello world\n");
			break;

		case 6:
			packet.setSmallFile(TcpPacket3::FLAG_BIT_ENCRYPTION | TcpPacket3::FLAG_BIT_CHECKSUM, "hello4.txt", "encryption+checksum\nhello world\nhello world\n");
			break;

		default:
			packet.set(TcpPacket3::FLAG_NONE, TcpPacket3::TYPE_NONE);
			break;
		}

		m_thread->sendPacket(packet);
		qDebug("DBG queue packet %d", i);
	}
}

void MainWindow::on_pushButton_connect_clicked(bool checked)
{
	qDebug("[UI] [MainWindow::on_pushButton_connect_clicked] %d", checked);

	if ( checked )
	{
		qDebug("DBG request connection");

		QString datagram = QString::asprintf("ew con %d ", m_tcpServer->serverPort()) + m_clientId;
		m_udpSocket->writeDatagram(datagram.toLocal8Bit(), QHostAddress(m_clientIp), DEVICE_UDP_PORT);
	}
	else
	{
		if ( m_thread )
		{
			qDebug("DBG terminate tcpsocketthread");
			m_thread->terminate();
		}
	}
}
