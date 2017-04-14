#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
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
	void on_pushButton_exit_clicked();

	void displayError(QLocalSocket::LocalSocketError socketError);

	void on_pushButton_send_clicked();

	void slotConnected();

	void on_pushButton_btn1_clicked();

	void on_pushButton_btn2_clicked();

	void on_pushButton_show_clicked();

	void on_pushButton_hide_clicked();

	void on_pushButton_up_clicked();

	void on_pushButton_down_clicked();

	void on_pushButton_right_clicked();

	void on_pushButton_left_clicked();

private:
	Ui::Dialog *ui;

	QLocalSocket *socket;

	QString m_cmdStr;

	void sendCommand(const QString & cmdStr);
};

#endif // DIALOG_H
