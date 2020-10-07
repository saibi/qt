#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tr_ko.load(":/lang_ko.qm");
    tr_de.load(":/lang_de.qm");

    retranslateUi();
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
        retranslateUi();
        break;
    default:
        break;
    }
}

void MainWindow::on_actionKorean_triggered()
{
    qApp->installTranslator(&tr_ko);
    update();
    qDebug() << "ko";
}

void MainWindow::on_actionDeutsch_triggered()
{
    qApp->installTranslator(&tr_de);
    update();
    qDebug() << "de";

}

void MainWindow::retranslateUi(void)
{
    ui->label->setText(tr("Hello World"));
    ui->label_2->setText(tr("Thank you"));
    ui->label_3->setText(tr("Fuck you"));
}

void MainWindow::on_actionEnglish_triggered()
{
    qApp->removeTranslator(&tr_ko);
    qApp->removeTranslator(&tr_de);
}
