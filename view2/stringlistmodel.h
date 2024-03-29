#ifndef _STRING_LIST_MODEL_H_
#define _STRING_LIST_MODEL_H_

#include <QAbstractListModel>
#include <QStringList>

class StringListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	StringListModel(const QStringList &strings, QObject *parent = 0) : QAbstractListModel(parent), stringList(strings) {}

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());


private:
	QStringList stringList;
};
#endif
