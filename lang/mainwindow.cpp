#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTranslator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label->setText(tr("Hello World !!!"));
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

void MainWindow::on_actionEnglish_triggered()
{
	//qApp->removeTranslator();
}

void MainWindow::on_actionKorean_triggered()
{
	QTranslator * translator = new QTranslator;
	translator->load("qrc:/lang_kr.qm");
	qApp->installTranslator(translator);

	qDebug("korean selected");
}
