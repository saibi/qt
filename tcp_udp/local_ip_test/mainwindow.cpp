#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QFileDialog>

#include "tcpsocketthread.h"

#define EW_READY "ew ready"
#define EW_HELLO "ew hello"
#define EW_DELAY "ew delay"
#define EW_CON "ew con"
#define EW_MYIP "ew myip"
#define EW_IP "ew ip"
#define EW_LIST "ew list"

#define DISCOVERY_SERVER_IP "172.16.5.50"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->groupBox_device->setEnabled(false);
	ui->widget_tcp_test->setEnabled(false);

	m_udpSocket = new QUdpSocket(this);
	m_udpSocket->bind(QHostAddress::Any, DISCOVERY_UDP_PORT);

#if QT_VERSION >= 0x050800
	connect(m_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams );
#else
	connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()) );
#endif

	m_tcpServer = new TcpServer(this);

	if ( !m_tcpServer->listen(QHostAddress::Any) )
	{
		qDebug() << "listen() error : " << m_tcpServer->errorString();
		m_tcpServer->close();
	}

	qDebug() << "listen port :" << m_tcpServer->serverPort();

#if QT_VERSION >= 0x050800
	connect(m_tcpServer, &TcpServer::signalConnected, this, &MainWindow::slot_clientConnected);
	connect(m_tcpServer, &TcpServer::signalDisconnected, this, &MainWindow::slot_clientDisconnected);
	connect(ui->widget_discoverList, &DiscoverListForm::signalDeviceSelected, this, &MainWindow::slot_clientSelected);

#else
	connect(m_tcpServer, SIGNAL(signalConnected(TcpSocketThread *)), this, SLOT(slot_clientConnected(TcpSocketThread *)));
	connect(m_tcpServer, SIGNAL(signalDisconnected()), this, SLOT(slot_clientDisconnected()));
	connect(ui->widget_discoverList, SIGNAL(signalDeviceSelected(QString, QString)), this, SLOT(slot_clientSelected(QString, QString)));
#endif
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::readPendingDatagrams()
{
	while (m_udpSocket->hasPendingDatagrams())
	{
		QHostAddress senderAddress;
		quint16 senderPort;

#if QT_VERSION >= 0x050800
		QNetworkDatagram datagram = m_udpSocket->receiveDatagram();

		senderAddress = datagram.senderAddress();
		senderPort = datagram.senderPort();
		QByteArray dgram = datagram.data();
#else
		qDebug("DBG %d", m_udpSocket->pendingDatagramSize());

		QByteArray dgram;
		dgram.resize(m_udpSocket->pendingDatagramSize());

		m_udpSocket->readDatagram(dgram.data(), dgram.size(),	&senderAddress, &senderPort);
#endif

		qDebug() << senderAddress << senderPort;
		qDebug() << dgram;

		processDatagram(dgram, &senderAddress);
	}
}

void MainWindow::processDatagram(const QByteArray &dgram, QHostAddress *senderAddress)
{
	QByteArray data = dgram.trimmed();

	if ( data.startsWith(EW_READY) )
	{
		QString id(data);
		id = id.replace(EW_READY, "").trimmed();
		ui->widget_discoverList->discoverUpdate(QDateTime::currentDateTime(), id, QHostAddress(senderAddress->toIPv4Address()).toString());
	}
	else if ( data.startsWith(EW_IP) )
	{
		QString destIP(data);
		destIP = destIP.replace(EW_IP, "").trimmed();

		QByteArray datagram = EW_HELLO;
		m_udpSocket->writeDatagram(datagram, QHostAddress(destIP), DISCOVERY_UDP_PORT);
	}

}

#if 0
void MainWindow::processDatagram(QNetworkDatagram datagram)
{
	QByteArray data = datagram.data().trimmed();

	qDebug() << datagram.senderAddress() << datagram.senderPort();
	qDebug() << datagram.data();

	if ( data.startsWith(EW_READY) )
	{
		QString id(data);
		id = id.replace(EW_READY, "").trimmed();
		ui->widget_discoverList->discoverUpdate(QDateTime::currentDateTime(), id, QHostAddress(datagram.senderAddress().toIPv4Address()).toString());
	}
	else if ( data.startsWith(EW_IP) )
	{
		QString destIP(data);
		destIP = destIP.replace(EW_IP, "").trimmed();

		QByteArray datagram = EW_HELLO;
		m_udpSocket->writeDatagram(datagram, QHostAddress(destIP), DISCOVERY_UDP_PORT);
	}
}
#endif

void MainWindow::on_pushButton_broadcast_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_broadcast_clicked]");

	ui->groupBox_device->setEnabled(false);
	ui->label_ip->clear();
	ui->widget_discoverList->clear();

	// broadcast
	//m_udpSocket->writeDatagram(EW_HELLO, QHostAddress::Broadcast, DISCOVERY_UDP_PORT);

	// use discovery server
	m_udpSocket->writeDatagram(EW_LIST, QHostAddress(DISCOVERY_SERVER_IP), DISCOVERY_UDP_PORT);
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
	if ( !ui->pushButton_connect->isChecked() )
		ui->pushButton_connect->setChecked(true);

}

void MainWindow::slot_clientDisconnected()
{
    m_thread = 0;
	qDebug() << "DBG clientDisConnected";

	if ( ui->pushButton_connect->isChecked() )
	{
		qDebug("DBG checked --> set false");
		ui->pushButton_connect->setChecked(false);
	}
	ui->widget_tcp_test->setEnabled(false);
	ui->pushButton_connect->setText("Connect");
}


void MainWindow::on_pushButton_delay_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_delay_clicked]");

	QByteArray datagram = QString(EW_DELAY " " + ui->lineEdit_delay->text()).toLocal8Bit();

	m_udpSocket->writeDatagram(datagram, QHostAddress(m_clientIp), DISCOVERY_UDP_PORT);
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

		QString first;
		first.sprintf("#%d;", counter++);
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
			packet.setCmdLine(TcpPacket3::FLAG_BIT_CHECKSUM, "set timestr=" + QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
			break;

		case 3:
			// small file test
		{
			QByteArray buf;

			buf += "plain\nhello world\nhello world\n";
			buf += QString("counter = %1\n").arg(counter).toLocal8Bit();
			packet.setSmallFile(TcpPacket3::FLAG_BIT_CHECKSUM, QString("hello%1.txt").arg(counter), buf );
//			packet.setSmallFile(TcpPacket3::FLAG_BIT_CHECKSUM, "hello2.txt", "checksum\nhello world\nhello world\n");
//			packet.setSmallFile(TcpPacket3::FLAG_BIT_ENCRYPTION, "hello3.txt", "encryption\nhello world\nhello world\n");
//			packet.setSmallFile(TcpPacket3::FLAG_BIT_ENCRYPTION | TcpPacket3::FLAG_BIT_CHECKSUM, "hello4.txt", "encryption+checksum\nhello world\nhello world\n");
		}

			break;

		case 4:
		{
		// big file packet test
#if 0
			QList<TcpPacket3> list;

			const char * data =
"Returns a byte array containing len bytes from\
this byte array, starting at position pos.\n\
Check if the Disconnect-request(FIN packet) has been received.\n\
User can confirm the reception of FIN packet as below.\n\
Returns the file size in bytes.\n\
If the file does not exist or cannot be fetched, 0 is returned.\n\
If the file is a symlink,the size of the target file is returned (not the symlink).\n\
Reads up to maxSize bytes from the device into data,\n\
and returns the number of bytes read or -1 if an error occurred.\n\
If there are no bytes to be read and there can never be more bytes available\n\
(examples include socket closed, pipe closed, sub-process finished),\n\
this function returns  -1.\n\
This function is called by QIODevice.  \n\
Reimplement this function when creating a subclass of QIODevice.\n\
When reimplementing this function it is important that this function\n\
reads all the required data before returning.\n\
This is required in order for QDataStream to be able to operate on the class.\n\
Please click View more.\n";

			QByteArray tmpFile;

			for (int i = 0 ; i < 100; ++i)
				tmpFile += data;

			tmpFile += "====================\n";

			list = TcpPacket3::makeBigFilePackets(TcpPacket3::FLAG_BIT_CHECKSUM, "bigfile.txt",
											   QString::asprintf("\n### %05d BIG FILE ###\n\n", i).toLocal8Bit() + tmpFile );

			if ( list.size() == 0 )
				break;

			for (int i = 0; i < list.size() - 1; ++i )
			{
				if ( !list.at(i).isValid() )
				{
					qDebug("DBG packet invalid #%d", i);
					continue;
				}
				m_thread->sendPacket(list.at(i));
			}

			packet = list.last();
#endif
		}
			break;

		case 5:
		{
			// file buffer test
			const char *t =
"Returns a byte\n\
array containing\n\
len bytes from\n\
this byte array,\n\
starting at\n\
position pos.\n\
Check if the\n\
Disconnect-request\n\
(FIN packet)\n\
has been received.\n\
User can confirm\n\
the reception\n\
of FIN packet as below.\n\
Please click View more.\n";

			QByteArray buf;

			QList<TcpPacket3> list;

			buf += t;

			list = TcpPacket3::makeFileBufferPackets(0, "test", buf);

			if ( list.size() == 0 )
				break;

			for (int i = 0; i < list.size() - 1; ++i )
			{
				if ( !list.at(i).isValid() )
				{
					qDebug("DBG packet invalid #%d", i);
					continue;
				}
				m_thread->sendPacket(list.at(i));
			}

			packet = list.last();
		}
			break;

		case 6:
		default:
			packet.set(TcpPacket3::FLAG_NONE, TcpPacket3::TYPE_NONE);
			break;
		}

		m_thread->sendPacket(packet);
	}
}

void MainWindow::on_pushButton_connect_clicked(bool checked)
{
	qDebug("[UI] [MainWindow::on_pushButton_connect_clicked] %d", checked);

	if ( checked )
	{
		qDebug("DBG request connection");

		QString datagram;
		datagram.sprintf(EW_CON " %d ", m_tcpServer->serverPort());
		datagram += m_clientId;
		m_udpSocket->writeDatagram(datagram.toLocal8Bit(), QHostAddress(m_clientIp), DISCOVERY_UDP_PORT);

		ui->pushButton_connect->setChecked(false);
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

void MainWindow::on_pushButton_dev_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_dev_clicked]");
}

void MainWindow::on_lineEdit_command_returnPressed()
{
	qDebug("[UI] [MainWindow::on_lineEdit_command_returnPressed] send command [%s]", qPrintable(ui->lineEdit_command->text()));

	TcpPacket3 packet;
	packet.setCmdLine(TcpPacket3::FLAG_BIT_CHECKSUM | TcpPacket3::FLAG_BIT_ENCRYPTION, ui->lineEdit_command->text());

	m_thread->sendPacket(packet);

	ui->lineEdit_command->clear();
}

void MainWindow::on_pushButton_browse_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_browse_clicked]");

	m_selectedFilePath = QFileDialog::getOpenFileName(this, "Select File", m_selectedFilePath, "All (*.*);;Text (*.txt)");

	if ( !m_selectedFilePath.isEmpty() )
	{
		ui->lineEdit_file->setText(m_selectedFilePath);
		qDebug("PATH : %s", qPrintable(m_selectedFilePath));
	}
}

static QByteArray readFileContents(const QString & path)
{
	QFile file(path);
	QByteArray contents;

	if ( file.open(QIODevice::ReadOnly) )
	{
		contents = file.readAll();
		file.close();
	}
	else
	{
		qDebug("DBG open error : %s", qPrintable(path));
	}
	return contents;
}

void MainWindow::on_pushButton_send_file_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_browse_clicked]");

	QByteArray contents = readFileContents(m_selectedFilePath);
	if ( contents.isEmpty() )
		return;

	TcpPacket3 packet;
	if ( packet.setSmallFile(TcpPacket3::FLAG_BIT_CHECKSUM | TcpPacket3::FLAG_BIT_ENCRYPTION, QFileInfo(m_selectedFilePath).fileName(), contents ) )
	{
		qDebug("send small file %d bytes", contents.size());
		m_thread->sendPacket(packet);
	}
}

void MainWindow::on_pushButton_send_big_file_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_send_big_file_clicked]");

	QByteArray contents = readFileContents(m_selectedFilePath);
	if ( contents.isEmpty() )
		return;

	QList<TcpPacket3> packetList;
	packetList = TcpPacket3::makeBigFilePackets(TcpPacket3::FLAG_BIT_CHECKSUM | TcpPacket3::FLAG_BIT_ENCRYPTION, QFileInfo(m_selectedFilePath).fileName(), contents );
	if ( packetList.size() > 0 )
	{
		for ( int i = 0; i < packetList.size(); ++i )
		{
			qDebug("send big file #%d", i);
			m_thread->sendPacket(packetList.at(i));
		}
	}
}

void MainWindow::on_pushButton_send_buffer_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_send_buffer_clicked]");

	QByteArray contents = readFileContents(m_selectedFilePath);
	if ( contents.isEmpty() )
		return;

	QList<TcpPacket3> packetList;
	packetList = TcpPacket3::makeFileBufferPackets(TcpPacket3::FLAG_BIT_CHECKSUM | TcpPacket3::FLAG_BIT_ENCRYPTION, QFileInfo(m_selectedFilePath).fileName(), contents );
	if ( packetList.size() > 0 )
	{
		for ( int i = 0; i < packetList.size(); ++i )
		{
			qDebug("send file buffer #%d", i);
			m_thread->sendPacket(packetList.at(i));
		}
	}
}
