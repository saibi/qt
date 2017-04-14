#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "qextserialport.h"
#include <QMessageBox>
#include <QString>





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    for ( int i = SLOT_SRC; i < SLOT_MAX ; i++ ) {
        port[i] = 0;
    }

    ui->setupUi(this);

    setup_combo_box();
    setup_buttons();
}


void MainWindow::setup_buttons()
{
    if ( port[::SLOT_SRC] != 0 ) {
        ui->pushButton_src_connect->setDisabled(true);
        ui->pushButton_src_disconnect->setEnabled(true);
    } else {
        ui->pushButton_src_connect->setDisabled(false);
        ui->pushButton_src_disconnect->setEnabled(false);
    }
    if ( port[::SLOT_DEST] != 0 ) {
        ui->pushButton_dest_connect->setDisabled(true);
        ui->pushButton_dest_disconnect->setEnabled(true);
    } else {
        ui->pushButton_dest_connect->setDisabled(false);
        ui->pushButton_dest_disconnect->setEnabled(false);
    }
}


void MainWindow::setup_combo_box()
{
    const char *item_list[] = {
#if defined(Q_OS_LINUX)
        "/dev/ttyUSB0",
        "/dev/ttyUSB1",
        "/dev/ttyS0",
        "/dev/ttyS1",

#elif defined(Q_OS_WIN32)
        "COM1",
        "COM2",
        "COM3",
        "COM4",
        "COM5",
        "COM6",
        "COM7",
        "COM8",

#endif
        0
    };

    QComboBox *p;

    p = ui->comboBox_src_port;
    for ( int i = 0; item_list[i] != 0 ; i++ ) {
        p->addItem(item_list[i]);
    }

    p = ui->comboBox_dest_port;
    for ( int i = 0; item_list[i] != 0 ; i++ ) {
        p->addItem(item_list[i]);
    }

    // button enable/disable




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/// baud rate string 을 BAUDxxxx 로 변경
static int get_baud_id(const QString &str)
{
    if ( str.compare("9600") == 0 ) {
        return BAUD9600;
    } else if ( str.compare("38400") == 0) {
        return BAUD38400;
    } else if ( str.compare("115200") == 0) {
        return BAUD115200;
    }

    return -1;
}

/// port 열기
/// \param id slot index, SLOT_SRC or SLOT_DEST
/// \return 0 success
int MainWindow::open_port(int id)
{
    QComboBox *p;

    if ( port[id] != 0 ) {
        return -ERR_ALREADY_CONNECTED;
    }

    if ( id == ::SLOT_SRC ) {
        p = ui->comboBox_src_speed;
    } else {
        p = ui->comboBox_dest_speed;
    }
    int baud = get_baud_id(p->currentText());
    if ( baud < 0 ) {
        return -ERR_INVALID_SPEED;
    }
    if ( id == ::SLOT_SRC ) {
        p = ui->comboBox_src_port;
    } else {
        p = ui->comboBox_dest_port;
    }
    port[id] = new QextSerialPort(qPrintable(p->currentText()));

    port[id]->setBaudRate((enum BaudRateType)baud);
    port[id]->setFlowControl(FLOW_OFF);
    port[id]->setParity(PAR_NONE);
    port[id]->setDataBits(DATA_8);
    port[id]->setStopBits(STOP_1);

    if ( !port[id]->open(QIODevice::ReadWrite) ) {
        // 에러 처리를 해야하는데 qextserialport 에서 에러 코드를 얻을 수 없는 것 같다.
        qDebug() << "Serial port open error : " << port[id]->errorString();
        delete port[id];
        port[id] = 0;
        return -ERR_OPEN_ERROR;
    }

    return 0;
}

/// port 닫기
/// \param id slot index, SLOT_SRC or SLOT_DEST
/// \return 0 success
int MainWindow::close_port(int id)
{
    if ( port[id] == 0 ) {
        return -ERR_NOT_OPENED;
    }

    port[id]->close();
    delete port[id];
    port[id] = 0;
    return 0;
}

/// error message 출력
/// \param parent
/// \param err error code
/// \param title title string
static void show_err_msg(QWidget *parent, int err, const char *title)
{
    err = abs(err);

    switch( err ) {

    case NO_ERR:
        break;

    case ERR_ALREADY_CONNECTED:
        QMessageBox::warning(parent, title, "Already connected.");
        break;

    case ERR_INVALID_SPEED:
        QMessageBox::warning(parent, title, "Invalid speed");
        break;

    case ERR_OPEN_ERROR:
        QMessageBox::warning(parent, title, "Open error");
        break;

    default:
        QMessageBox::warning(parent, title, QString("Error %1").arg(err));
        break;

    }
}

/// message dump
///
void format_msg(char *src, int len, QString &out_str)
{
        int i = 0, col = 0;
        QString tmp_str, tmp_hex;

        while( i < len ) {

                tmp_str.clear();
                for (col = 0; col < 16 && (i + col) < len; col++ ) {
                        tmp_hex.sprintf("%02x ", (unsigned char)src[i+col]);
                        out_str += tmp_hex;
                        if ( src[i+col] < ' ' || src[i+col] > 127 ) {
                                tmp_str += ' ';
                        } else {
                                tmp_str += src[i+col];
                        }
                }
                for (int remain = col; remain < 16; remain ++ ) {
                        out_str += "   ";
                }
                tmp_str += ' |\n';
                out_str += "| ";
                out_str += tmp_str;
                i += col;
        }
}

void MainWindow::msg_dump()
{
        char buf[8192];
        int ret;

        if ( QObject::sender()== port[SLOT_SRC] ) {
                ret = port[SLOT_SRC]->read(buf, sizeof(buf));
                qDebug() << "read src = " << ret << " bytes.";
                buf[ret] = 0;

                QString out;
                format_msg(buf, ret, out);

                ui->textEdit_src->append(out);
                if ( port[SLOT_DEST] ) {
                        port[SLOT_DEST]->write(buf, ret);
                }
        } else {
                ret = port[SLOT_DEST]->read(buf, sizeof(buf));
                qDebug() << "read dest = " << ret << " bytes.";
                buf[ret] = 0;

                QString out;
                format_msg(buf, ret, out);

                ui->textEdit_dest->append(out);
                if ( port[SLOT_SRC] ) {
                        port[SLOT_SRC]->write(buf, ret);
                }
        }
}

void MainWindow::on_pushButton_src_connect_clicked()
{
    int err = open_port(SLOT_SRC);

    if ( err < 0 ) {
        show_err_msg(this, err, "Source Connect");
        return;
    }

    setup_buttons();

    connect(port[SLOT_SRC], SIGNAL(readyRead()), this, SLOT(msg_dump()));
}

void MainWindow::on_pushButton_src_disconnect_clicked()
{
    close_port(SLOT_SRC);
    setup_buttons();
    disconnect(port[SLOT_SRC], SIGNAL(readyRead()), this, SLOT(msg_dump()));
}

void MainWindow::on_pushButton_dest_connect_clicked()
{
    int err = open_port(SLOT_DEST);

    if ( err < 0 ) {
        show_err_msg(this, err, "Destination Connect");
        return;
    }
    setup_buttons();
    connect(port[SLOT_DEST], SIGNAL(readyRead()), this, SLOT(msg_dump()));
}

void MainWindow::on_pushButton_dest_disconnect_clicked()
{
    close_port(SLOT_DEST);
    setup_buttons();
    disconnect(port[SLOT_DEST], SIGNAL(readyRead()), this, SLOT(msg_dump()));
}
