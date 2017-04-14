#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserialport.h"
#include <QLabel>


enum error_id {
    NO_ERR,
    ERR_ALREADY_CONNECTED = 1000,
    ERR_INVALID_SPEED,
    ERR_OPEN_ERROR,
    ERR_NOT_CONNECTED,
};


#define CHAR_FS 0x1c
#define CHAR_GS 0x1d
#define CHAR_ACK 0x06
#define CHAR_NAK 0x15
#define CHAR_ESC 0x1b
#define CHAR_RS 0x1e
#define CHAR_CR 0x0d
#define CHAR_LF 0x0a

#define STR_FS "\x1c"
#define STR_GS "\x1d"
#define STR_ACK "\x06"
#define STR_NAK "\x15"
#define STR_RS "\x1e"
#define STR_CR "\x0d"
#define STR_LF "\x0a"


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
    QextSerialPort *port;
    QLabel status_msg;

    void setup_combo_box();
    int open_port();
    int close_port();
    void setup_status_msg();

private slots:
    void on_actionSend_buffer_triggered();
    void on_actionSend_triggered();
    void on_actionDisconnect_triggered();
    void on_actionConnect_triggered();
    void packet_dump();
};

#endif // MAINWINDOW_H
