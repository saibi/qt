#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        setCentralWidget(&spinBox);
        spinBox.setRange(0, 0x0fffffff);

        namedLabel = new QLabel(tr("Named :"));
        namedLineEdit = new QLineEdit;
        namedLabel->setBuddy(namedLineEdit);

}

MainWindow::~MainWindow()
{
        delete ui;
}
