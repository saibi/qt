#include "characters.h"
#include <QDebug>

using namespace entities;

Characters::Characters(QObject *parent)
	: QAbstractListModel(parent)
{
}

int Characters::rowCount(const QModelIndex &parent) const
{
	// For list models only the root node (an invalid parent) should return the list's size. For all
	// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
	if (parent.isValid())
		return 0;

	return m_list.count();
}

QVariant Characters::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if ( role == Qt::DisplayRole && index.column() == 0 )
		return m_list.at(index.row());

	return QVariant();
}

void Characters::add(const QString & name)
{
	if ( ! m_list.contains(name) )
	{
		beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
		m_list.append(name);
		endInsertRows();

		qDebug() << "add " + name;
	}
}

void Characters::remove(const QString & name)
{
	int index = m_list.indexOf(name);
	if ( index > -1 )
	{
		beginRemoveRows(QModelIndex(), index, index);
		m_list.removeAt(index);
		endRemoveRows();
	}
}
