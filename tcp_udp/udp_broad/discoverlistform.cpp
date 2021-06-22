#include "discoverlistform.h"
#include "ui_discoverlistform.h"

DiscoverListForm::DiscoverListForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DiscoverListForm)
{
	ui->setupUi(this);

	m_discoverRecord.setHorizontalHeaderLabels(QStringList({"Date & Time", "Name", "IP"}));

	m_discoverProxy.setSourceModel(&m_discoverRecord);
	ui->tableView->setModel(&m_discoverRecord);
}

DiscoverListForm::~DiscoverListForm()
{
	delete ui;
}

void DiscoverListForm::discoverUpdate(const QDateTime & timeStamp, const QString & name, const QString & address)
{
	auto itemT = new QStandardItem;
	auto itemN = new QStandardItem;
	auto itemA = new QStandardItem;

	itemT->setData(timeStamp, Qt::DisplayRole);
	itemN->setData(name, Qt::DisplayRole);
	itemA->setData(address, Qt::DisplayRole);

	QList <QStandardItem *> row;
	row << itemT << itemN << itemA;

	m_discoverRecord.appendRow(row);
}

void DiscoverListForm::clear()
{
	m_discoverRecord.clear();
}
