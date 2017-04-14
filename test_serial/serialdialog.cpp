#include "serialdialog.h"
#include "ui_serialdialog.h"

SerialDialog::SerialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialDialog)
{
    ui->setupUi(this);

    ui->pushButton_add->setEnabled(false);

    ui->comboBox_baudRate->addItem(QString("9600"), 9600);
    ui->comboBox_baudRate->addItem(QString("38400"), 38400);
    ui->comboBox_baudRate->addItem(QString("115200"), 115200);

    ui->comboBox_baudRate->setCurrentIndex(0);
}

SerialDialog::~SerialDialog()
{
    delete ui;
}

void SerialDialog::changeEvent(QEvent *e)
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

void SerialDialog::on_lineEdit_portName_textEdited(QString portName)
{
    if (portName.trimmed().isEmpty())
    {
        ui->pushButton_add->setEnabled(false);
    }
    else
    {
        ui->pushButton_add->setEnabled(true);
    }
}

void SerialDialog::on_pushButton_add_clicked()
{
    _portName = ui->lineEdit_portName->text();
    _baudRate = ui->comboBox_baudRate->itemData(ui->comboBox_baudRate->currentIndex()).toInt();

    accept();
}

void SerialDialog::on_pushButton_cancel_clicked()
{
    reject();
}
