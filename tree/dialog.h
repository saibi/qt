#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>

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
    Ui::Dialog *m_ui;
};

#endif // DIALOG_H
