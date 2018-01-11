#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonLoad_clicked();
};

#endif // DIALOG_H
