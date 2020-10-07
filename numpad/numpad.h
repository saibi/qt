#ifndef NUMPAD_H
#define NUMPAD_H

#include <QDialog>

namespace Ui {
    class numpad;
}

class numpad : public QDialog {
    Q_OBJECT
public:
    numpad(QWidget *parent = 0);
    ~numpad();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::numpad *ui;

private slots:
    void on_btn_clicked();
};

#endif // NUMPAD_H
