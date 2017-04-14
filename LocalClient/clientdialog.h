#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>

#include <QLocalSocket>

namespace Ui {
class ClientDialog;
}

class ClientDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ClientDialog(QWidget *parent = 0);
	~ClientDialog();

private slots:
	void displayError(QLocalSocket::LocalSocketError socketError);
	void slotConnected();

	void on_pushButton_clicked();

private:
	Ui::ClientDialog *ui;

	QLocalSocket * m_socket;
};

#endif // CLIENTDIALOG_H
