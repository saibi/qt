#include "discoverlistform.h"
#include "ui_discoverlistform.h"

DiscoverListForm::DiscoverListForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DiscoverListForm)
{
	ui->setupUi(this);

	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_discoverRecord.setHorizontalHeaderLabels(QStringList() << "Name" << "IP" << "Date & Time");
	m_discoverProxy.setSourceModel(&m_discoverRecord);
	ui->tableView->setModel(&m_discoverRecord);
}

DiscoverListForm::~DiscoverListForm()
{
	delete ui;
}

void DiscoverListForm::discoverUpdate(const QDateTime & timeStamp, const QString & name, const QString & address)
{

	// check duplicated name
	for ( int row = 0; row < m_discoverRecord.rowCount(); ++row)
	{
		if ( m_discoverRecord.index(row, 0).data().toString() == name )
		{
			qDebug() << "DBG " << row << m_discoverRecord.index(row, 0).data() << " remove & insert";

			m_discoverRecord.removeRow(row);
			break;
		}
	}

    QStandardItem * itemT = new QStandardItem;
    QStandardItem * itemN = new QStandardItem;
    QStandardItem * itemA = new QStandardItem;

	itemT->setData(timeStamp, Qt::DisplayRole);
	itemN->setData(name, Qt::DisplayRole);
	itemA->setData(address, Qt::DisplayRole);

	QList <QStandardItem *> row;
	row << itemN << itemA << itemT;

	m_discoverRecord.appendRow(row);

	ui->tableView->resizeColumnToContents(0);
	ui->tableView->resizeColumnToContents(1);

}

void DiscoverListForm::clear()
{
	m_discoverRecord.clear();
    m_discoverRecord.setHorizontalHeaderLabels(QStringList() << "Name" << "IP" << "Date & Time");
}

void DiscoverListForm::on_tableView_doubleClicked(const QModelIndex &index)
{
	qDebug("[UI] [DiscoverListForm::on_tableView_doubleClicked]");

	qDebug() << index.data() << index.sibling(index.row(), 1).data();  // index.siblingAtColumn(1).data(); <- qt 5.15.x

	emit signalDeviceSelected(index.sibling(index.row(), 0).data().toString(), index.sibling(index.row(), 1).data().toString());
}
