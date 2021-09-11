#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "tcpsocketthread.h"

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
	void on_pushButton_test_clicked();

private:
    Ui::Dialog *ui;

	TcpSocketThread *m_socketThread;
};

#endif // DIALOG_H
