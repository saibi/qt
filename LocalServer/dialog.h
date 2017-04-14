#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLocalServer>
#include <QLocalSocket>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

private slots:
	void slotConnected();
	void slotReadCommand();

	void on_pushButton_clicked();

signals:
	void signalCommandReceived(const QString & cmd);

private:
	Ui::Dialog *ui;

	QLocalServer * m_server;
	QLocalSocket * m_socket;
};

#endif // DIALOG_H
