#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include <QDialog>

namespace Ui {
    class SerialDialog;
}

class SerialDialog : public QDialog {
    Q_OBJECT
public:
    SerialDialog(QWidget *parent = 0);
    ~SerialDialog();

    const QString& getPortName() {return _portName;}
    int getBaudRate() {return _baudRate;}

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SerialDialog *ui;

    QString _portName;
    int _baudRate;

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_add_clicked();
    void on_lineEdit_portName_textEdited(QString );
};

#endif // SERIALDIALOG_H
