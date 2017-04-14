#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>

namespace Ui {
class Client;
}

class Client : public QDialog
{
	Q_OBJECT

public:
	explicit Client(QWidget *parent = 0);
	~Client();

private slots:
	void on_pushButton_send_clicked();

	void slotConnect();

	void on_pushButton_id0_clicked();

	void on_pushButton_id1_clicked();

	void on_pushButton_up_clicked();

	void on_pushButton_left_clicked();

	void on_pushButton_right_clicked();

	void on_pushButton_down_clicked();

	void on_pushButton_show_clicked();

	void on_pushButton_hide_clicked();

private:
	Ui::Client *ui;
};

#endif // CLIENT_H
