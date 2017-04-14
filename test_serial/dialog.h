#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "serialmanager.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Dialog *ui;

    SerialManager _serialManager;

public slots:
    void handlePacketReceived(int id);

private slots:
    void on_comboBox_send_currentIndexChanged(QString );
    void on_pushButton_send_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_add_clicked();
};

#endif // DIALOG_H
