#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setup_port_combo();


}

void MainWindow::setup_port_combo()
{
    char *item_list[] = {
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
