#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserialport.h"

enum error_id {
    NO_ERR,
    ERR_ALREADY_CONNECTED = 1000,
    ERR_INVALID_SPEED,
    ERR_OPEN_ERROR,
    ERR_NOT_OPENED,
};

enum slot_id {
    SLOT_SRC,
    SLOT_DEST,
    SLOT_MAX
};


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

    void setup_combo_box();
    void setup_buttons();
    int open_port(int id);
    int close_port(int id);

    QextSerialPort *port[SLOT_MAX];

private slots:
    void on_pushButton_dest_disconnect_clicked();
    void on_pushButton_dest_connect_clicked();
    void on_pushButton_src_disconnect_clicked();
    void on_pushButton_src_connect_clicked();

    void msg_dump();
};

#endif // MAINWINDOW_H
