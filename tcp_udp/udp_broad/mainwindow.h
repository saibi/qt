#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QUdpSocket>
#include <QNetworkDatagram>

#include "tcpserver.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected:
	void readPendingDatagrams();
	void processDatagram(QNetworkDatagram datagram);

	void slot_clientSelected(const QString & id, const QString & ip);

private slots:
	void on_pushButton_broadcast_clicked();

	void on_pushButton_delay_clicked();

	void slot_clientConnected(TcpSocketThread *thread);
	void slot_clientDisconnected();

	void on_pushButton_test_clicked();

	void on_pushButton_connect_clicked(bool checked);


	void on_pushButton_dev_clicked();

	void on_lineEdit_command_returnPressed();

	void on_pushButton_browse_clicked();

	void on_pushButton_send_file_clicked();

	void on_pushButton_send_big_file_clicked();

	void on_pushButton_send_buffer_clicked();

private:
	Ui::MainWindow *ui;

	QUdpSocket *m_udpSocket = nullptr;
	TcpServer *m_tcpServer = nullptr;

	TcpSocketThread *m_thread = nullptr;
	QString m_clientIp;
	QString m_clientId;

	QString m_selectedFilePath;

	enum InternalConstants {
		DISCOVERY_UDP_PORT = 8279,
	};	
};
#endif // MAINWINDOW_H
