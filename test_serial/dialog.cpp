#include "dialog.h"
#include "ui_dialog.h"

#include "serialdialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    _serialManager(this)
{
    ui->setupUi(this);

    ui->listWidget_serial->clear();

    ui->pushButton_send->setEnabled(false);

    connect(&_serialManager, SIGNAL(packetReceived(int)), this, SLOT(handlePacketReceived(int)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog::on_pushButton_add_clicked()
{
    SerialDialog dlg;

    if (dlg.exec() == QDialog::Accepted)
    {
        QString portName = dlg.getPortName();
        int baudRateNum = dlg.getBaudRate();
        BaudRateType baudRate;

        switch (baudRateNum)
        {
        case 9600:
            baudRate = BAUD9600;
            break;

        case 38400:
            baudRate = BAUD38400;
            break;

        case 115200:
            baudRate = BAUD115200;
            break;

        default:
            baudRate = BAUD9600;
        }

        int id = 0;

        if (_serialManager.start(id, portName, baudRate) == true)
        {
            // ok
            QString text = portName + QString(" - ID:") + QString::number(id) + QString(", BAUDRATE:") + QString::number(baudRateNum);

            QListWidgetItem* pItem = new QListWidgetItem(text);
            pItem->setData(Qt::UserRole, id);

            ui->listWidget_serial->addItem(pItem);

            ui->comboBox_send->addItem(text, id);
            ui->comboBox_send->setCurrentIndex(ui->comboBox_send->findText(text));
        }
        else
        {
            // error
            qDebug("Error - _serialManager.start \n");
        }
    }
}

void Dialog::on_pushButton_delete_clicked()
{
    QListWidgetItem* pItem = ui->listWidget_serial->takeItem(ui->listWidget_serial->currentRow());
    if (pItem == 0)
        return;

    int id = pItem->data(Qt::UserRole).toInt();

    if (_serialManager.stop(id) == false)
    {
        // error
        qDebug("Error - _serialManager.stop \n");
    }

    ui->comboBox_send->removeItem(ui->comboBox_send->findText(pItem->text()));
}

void Dialog::on_pushButton_send_clicked()
{
    Packet packet;

    packet.append(RC_START_MSG);
    packet.append(ui->lineEdit_send->text());
    packet.append(RC_END_MSG);

    int id = ui->comboBox_send->itemData(ui->comboBox_send->currentIndex()).toInt();

    if (_serialManager.sendPacket(id, packet) == false)
    {
        // error
        qDebug("Error - _serialManager.sendPacket - ID: [%d]\n", id);
    }
}

void Dialog::on_comboBox_send_currentIndexChanged(QString portName)
{
    if (portName.trimmed().isEmpty())
    {
        ui->pushButton_send->setEnabled(false);
    }
    else
    {
        ui->pushButton_send->setEnabled(true);
    }
}

void Dialog::handlePacketReceived(int id)
{
    Packet packet;

    if (_serialManager.recvPacket(id, packet) == true)
    {
        QString portName = _serialManager.getPortName(id);
        QString timeString = packet.getCreatedTime().toString(QString("hh:mm:ss.zzz"));
        QString packetString = packet.getAsFormatString();

        QString text = QString("\n-----------------\n") +
                       QString("** RECV ** - [") + portName + QString("] - Time: ") + timeString +
                       QString("\n-----------------\n") +
                       packetString;

        //ui->plainTextEdit_log->setPlainText(ui->plainTextEdit_log->toPlainText().right(100) + text);
        ui->plainTextEdit_log->appendPlainText(text);
    }
}
