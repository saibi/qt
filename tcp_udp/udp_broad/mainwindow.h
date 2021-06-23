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

	void sendReqConnect(const QString & id, const QString & ip);

private slots:
	void on_pushButton_broadcast_clicked();

private:
	Ui::MainWindow *ui;

	QUdpSocket *m_udpSocket = nullptr;
	TcpServer *m_tcpServer = nullptr;
};
#endif // MAINWINDOW_H
