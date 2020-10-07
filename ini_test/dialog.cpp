#include "dialog.h"
#include "ui_dialog.h"

#include <QSettings>
#include <QMap>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButtonLoad_clicked()
{
    QSettings setting(QSettings::IniFormat, QSettings::UserScope, "Huvitz", "Settings");

    // filename
    ui->labelFilename->setText(QString("Load: ") + setting.fileName());

    int dataI;
    double dataF;
    QString dataS;

    // Normal
    dataI = setting.value("int", 0).toInt();
    dataF = setting.value("float", 0).toDouble();
    dataS = setting.value("string", "").toString();

    ui->lineEditInt->setText(QString::number(dataI));
    ui->lineEditFloat->setText(QString::number(dataF));
    ui->lineEditString->setText(dataS);

    // Group A
    setting.beginGroup("group_a");

    dataI = setting.value("int", 0).toInt();
    dataF = setting.value("float", 0).toDouble();
    dataS = setting.value("string", "").toString();

    ui->lineEditIntA->setText(QString::number(dataI));
    ui->lineEditFloatA->setText(QString::number(dataF));
    ui->lineEditStringA->setText(dataS);

    setting.endGroup();

    // Group B
    setting.beginGroup("group_b");

    dataI = setting.value("int", 0).toInt();
    dataF = setting.value("float", 0).toDouble();
    dataS = setting.value("string", "").toString();

    ui->lineEditIntB->setText(QString::number(dataI));
    ui->lineEditFloatB->setText(QString::number(dataF));
    ui->lineEditStringB->setText(dataS);

    setting.endGroup();

    // Map
    QMap<QString, QVariant> map;

    map = setting.value("map").toMap();

    ui->lineEditMap0->setText(map.value("data0").toString());
    ui->lineEditMap1->setText(map.value("data1").toString());
    ui->lineEditMap2->setText(map.value("data2").toString());
    ui->lineEditMap3->setText(map.value("data3").toString());
    ui->lineEditMap4->setText(map.value("data4").toString());
}

void Dialog::on_pushButtonSave_clicked()
{
    QSettings setting(QSettings::IniFormat, QSettings::UserScope, "Huvitz", "Settings");

    // filename
    ui->labelFilename->setText(QString("Save: ") + setting.fileName());

    int dataI;
    double dataF;
    QString dataS;

    // Normal
    dataI = ui->lineEditInt->text().toInt();
    dataF = ui->lineEditFloat->text().toDouble();
    dataS = ui->lineEditString->text();

    setting.setValue("int", dataI);
    setting.setValue("float", dataF);
    setting.setValue("string", dataS);

    // Group A
    setting.beginGroup("group_a");

    dataI = ui->lineEditIntA->text().toInt();
    dataF = ui->lineEditFloatA->text().toDouble();
    dataS = ui->lineEditStringA->text();

    setting.setValue("int", dataI);
    setting.setValue("float", dataF);
    setting.setValue("string", dataS);

    setting.endGroup();

    // Group B
    setting.beginGroup("group_b");

    dataI = ui->lineEditIntB->text().toInt();
    dataF = ui->lineEditFloatB->text().toDouble();
    dataS = ui->lineEditStringB->text();

    setting.setValue("int", dataI);
    setting.setValue("float", dataF);
    setting.setValue("string", dataS);

    setting.endGroup();

    // Map
    QMap<QString, QVariant> map;

    map.insert("data0", ui->lineEditMap0->text());
    map.insert("data1", ui->lineEditMap1->text());
    map.insert("data2", ui->lineEditMap2->text());
    map.insert("data3", ui->lineEditMap3->text());
    map.insert("data4", ui->lineEditMap4->text());

    setting.setValue("map", map);

    setting.sync();
}
