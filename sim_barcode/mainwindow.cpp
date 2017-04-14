#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qextserialport.h"
#include <qDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    port = 0;
    ui->setupUi(this);

    status_msg.setText(tr("Not connected"));
    ui->statusBar->addPermanentWidget(&status_msg);

    setup_combo_box();
}

MainWindow::~MainWindow()
{
    delete ui;
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

    p = ui->comboBox_port;
    for ( int i = 0; item_list[i] != 0 ; i++ ) {
        p->addItem(item_list[i]);
    }
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
/// \return 0 success
int MainWindow::open_port()
{
    QComboBox *p;

    if ( port != 0 ) {
        return -ERR_ALREADY_CONNECTED;
    }

    p = ui->comboBox_speed;

    int baud = get_baud_id(p->currentText());
    if ( baud < 0 ) {
        return -ERR_INVALID_SPEED;
    }

    p = ui->comboBox_port;

    port = new QextSerialPort(qPrintable(p->currentText()));
    port->setBaudRate((enum BaudRateType)baud);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    if ( !port->open(QIODevice::ReadWrite) ) {
        // 에러 처리를 해야하는데 qextserialport 에서 에러 코드를 얻을 수 없는 것 같다.
        qDebug() << "Serial port open error : " << port->errorString();
        delete port;
        port = 0;
        return -ERR_OPEN_ERROR;
    }

    return 0;
}

/// port 닫기
/// \return 0 success
int MainWindow::close_port()
{
    if ( port == 0 ) {
        return -ERR_NOT_CONNECTED;
    }

    port->close();
    delete port;
    port = 0;
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

    case ERR_NOT_CONNECTED:
        QMessageBox::warning(parent, title, "Not connected");
        break;

    default:
        QMessageBox::warning(parent, title, QString("Error %1").arg(err));
        break;

    }
}

void MainWindow::setup_status_msg()
{
    if ( port != 0 ) {
        status_msg.setText(tr("Connected") + "|" + ui->comboBox_port->currentText() + "|" + ui->comboBox_speed->currentText());
    } else {
        status_msg.setText(tr("Not connected"));
    }
}

void MainWindow::on_actionConnect_triggered()
{
    int err = open_port();

    if ( err < 0 ) {
        show_err_msg(this, err, "Connect");
        return;
    }


    setup_status_msg();
    connect(port, SIGNAL(readyRead()), this, SLOT(packet_dump()));
}

void MainWindow::on_actionDisconnect_triggered()
{
    int err = close_port();

    if ( err < 0 ) {
        show_err_msg(this, err, "Disconnect");
    }
    setup_status_msg();
    disconnect(port, SIGNAL(readyRead()), this, SLOT(packet_dump()));
}


// 패킷 특수 문자
static const char * get_conv_str(char code)
{
    switch (code) {
    case CHAR_FS:
        return "<FS>\n";
    case CHAR_GS:
        return "<GS>\n";
    case CHAR_ACK:
        return "<ACK>";
    case CHAR_NAK:
        return "<NAK>";
    case CHAR_ESC:
        return "<ESC>";
    case CHAR_RS:
        return "<RS>";
    case CHAR_CR:
        return "<CR>\n";
    case CHAR_LF:
        return "<LF>\n";

    }
    return NULL;
}

// packet 을 보기 쉽게 변환
static void format_packet(char *src, int len, QString &out)
{
    int i = 0;
    QString tmp_str;
    const char *r_str;

    for( i = 0 ; i < len; i++) {
        tmp_str.clear();

        if ( (r_str = get_conv_str(src[i])) != NULL) {
            tmp_str.sprintf("%s", r_str);
        } else if ( src[i] < ' ' ) {
            tmp_str.sprintf("<%02X>", src[i]);
        } else {
            tmp_str.sprintf("%c", src[i]);
        }
        out += tmp_str;
    }
}

void MainWindow::packet_dump()
{
    char buf[64*1024];
    int ret;
    QString out;

    ret = port->read(buf, sizeof(buf));
    qDebug() << "read " << ret << "bytes data.";
    buf[ret] = 0;

    format_packet(buf, ret, out);
    ui->plainTextEdit->appendPlainText(out);
}

/// String 을 바코드용 패킷으로 변환
static int conv_string_to_barcode_packet(QString str, char *packet, int max)
{
    int src_idx, dest_idx;

    max--; // to store terminated NULL character

    for ( src_idx = 0, dest_idx = 0 ; src_idx < str.length() && dest_idx < max ;) {
        qDebug() << src_idx << " " << str.at(src_idx).toAscii();

        if ( str.at(src_idx).toAscii() == 0x5c ) {
            if ( str.at(src_idx+1).toAscii() == 'r' ) {
                packet[dest_idx++] = '\r';
                src_idx+=2;
            } else if ( str.at(src_idx+1).toAscii() == 'n') {
                packet[dest_idx++] = '\n';
                src_idx+=2;
            } else {
                packet[dest_idx++] = str.at(src_idx++).toAscii();
            }
        } else {
            packet[dest_idx++] = str.at(src_idx++).toAscii();
        }
    }
    packet[dest_idx] = 0;
    return dest_idx;
}

void MainWindow::on_actionSend_triggered()
{
    char fake_packet[1024]= {0, };
    int ret;

    if ( port == 0 ) {
        show_err_msg(this, ERR_NOT_CONNECTED, "send");
        return;
    }
    qDebug() << "barcode str = " << ui->lineEdit->text() << " ret = " << conv_string_to_barcode_packet(ui->lineEdit->text(), fake_packet, sizeof(fake_packet));

    ret = port->write(fake_packet, strlen(fake_packet) );
    qDebug() << "write " << strlen(fake_packet) << "bytes.";
}
