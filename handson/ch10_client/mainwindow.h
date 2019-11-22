#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QTcpSocket>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void printMessage(QString message);

private slots:
	void on_pushButton_connect_clicked();

	void socketConnected();
	void socketDisconnected();
	void socketReadyRead();
	void on_pushButton_send_clicked();

private:
	Ui::MainWindow *ui;

	bool connectedToHost;
	QTcpSocket * socket;
};

#endif // MAINWINDOW_H
