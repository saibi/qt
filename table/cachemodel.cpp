#include "cachemodel.h"

cacheModelClass::cacheModelClass(QObject *parent) : QAbstractTableModel(parent) {
	int count = 0;

	for ( int r = 0 ; r < NOSETS; r++) {
		for (int c = 0 ; c < LINESIZE; c++ ) {
			array[r][c] = count++;
		}
	}
}

int cacheModelClass::rowCount(const QModelIndex &parent) const {
	return NOSETS;
}

int cacheModelClass::columnCount(const QModelIndex &parent) const {
	return LINESIZE;
}

QVariant cacheModelClass::data(const QModelIndex &index, int role) const {
	if ( !index.isValid() || role != Qt::DisplayRole) {
		return QVariant();
	}

	return array[index.row()][index.column()];
}


QVariant cacheModelClass::headerData(int section, Qt::Orientation orientation, int role ) const {
	if ( role != Qt::DisplayRole) {
		return QVariant();
	}

	if ( orientation == Qt::Horizontal) {
		return QString("Word %1").arg(section);
	} else {
		return QString("Set %1").arg(section);
	}
}


int cacheModelClass::setItem(int row, int col, int datum) {
	if ( row < 0 || col < 0 || row >=NOSETS || col >= LINESIZE ) {
		return 0;
	}


	array[row][col] = datum;

	emit dataChanged( createIndex(row, col), createIndex(row, col));
	return 1;
}
