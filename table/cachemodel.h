#ifndef CACHEMODEL_H
#define CACHEMODEL_H

#include <QAbstractTableModel>
#include <QVariant>

#define NOSETS 8
#define LINESIZE 4


class cacheModelClass : public QAbstractTableModel {
	Q_OBJECT
private:
	int array[NOSETS][LINESIZE];

public:
	cacheModelClass(QObject *parent=0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	int setItem(int row, int col, int datum);

};

#endif
