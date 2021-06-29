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

	void on_pushButton_connect_clicked();

	void slot_clientConnected(TcpSocketThread *thread);
	void slot_clientDisconnected();


private:
	Ui::MainWindow *ui;

	QUdpSocket *m_udpSocket = nullptr;
	TcpServer *m_tcpServer = nullptr;

	TcpSocketThread *m_thread = nullptr;
	QString m_clientIp;
	QString m_clientId;
};
#endif // MAINWINDOW_H
