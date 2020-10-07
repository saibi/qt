#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDate>

void add_line(QAbstractItemModel *model, const QString &subject, const QString &sender, const QDateTime &date)
{
	model->insertRow(0);
	model->setData(model->index(0, 0), subject);
	model->setData(model->index(0, 1), sender);
	model->setData(model->index(0, 2), date);
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(0, 3, parent);

    model->setHeaderData(0, Qt::Horizontal, tr("Subject"));
    model->setHeaderData(1, Qt::Horizontal, tr("Sender"));
    model->setHeaderData(2, Qt::Horizontal, tr("Date"));

    add_line(model, "Happy new year", "Grace K", QDateTime(QDate(2009, 11, 10), QTime(15, 00)));
    add_line(model, "year", "hahaha", QDateTime(QDate(2009, 11, 9), QTime(9, 10)));
    add_line(model, "play", "ball", QDateTime(QDate(2009, 11, 11), QTime(12, 34)));


    ui->treeView->setModel(model);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::msg_exit()
{
	QMessageBox::warning(this, tr("Exit"),
		tr("Do you really want to exit?"),
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No,
		QMessageBox::Cancel | QMessageBox::Escape);
	close();
}
