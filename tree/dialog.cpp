#include "dialog.h"
#include "ui_dialog.h"
#include <QStandardItemModel>
#include <QDate>
#include <QTime>
#include <QAbstractItemModel>

void add_line(QAbstractItemModel *model, const QString &subject, const QString &sender, const QDateTime &date)
{
	model->insertRow(0);
	model->setData(model->index(0, 0), subject);
	model->setData(model->index(0, 1), sender);
	model->setData(model->index(0, 2), date);
}


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Dialog)
{
    m_ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(0, 3, parent);

    model->setHeaderData(0, Qt::Horizontal, tr("Subject"));
    model->setHeaderData(1, Qt::Horizontal, tr("Sender"));
    model->setHeaderData(2, Qt::Horizontal, tr("Date"));

    add_line(model, "Happy new year", "Grace K", QDateTime(QDate(2009, 11, 10), QTime(15, 00)));
    add_line(model, "year", "hahaha", QDateTime(QDate(2009, 11, 9), QTime(9, 10)));
    add_line(model, "play", "ball", QDateTime(QDate(2009, 11, 11), QTime(12, 34)));

   m_ui->treeView->setModel(model);
}

Dialog::~Dialog()
{
    delete m_ui;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
