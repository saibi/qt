#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#if QT_VERSION >= 0x050800
#include <QNetworkDatagram>
#endif

#include <QUdpSocket>

#include "tcpserver.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
	~MainWindow();

protected slots:
	void readPendingDatagrams();
	void processDatagram(const QByteArray &dgram, QHostAddress * senderAddress);
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

    QUdpSocket *m_udpSocket = 0;
    TcpServer *m_tcpServer = 0;

    TcpSocketThread *m_thread = 0;
	QString m_clientIp;
	QString m_clientId;

	QString m_selectedFilePath;

	enum InternalConstants {
		DISCOVERY_UDP_PORT = 8279,
	};	
};
#endif // MAINWINDOW_H
