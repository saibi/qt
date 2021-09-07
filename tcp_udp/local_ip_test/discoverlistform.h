#ifndef DISCOVERLISTFORM_H
#define DISCOVERLISTFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHostAddress>
#include <QDateTime>

namespace Ui {
class DiscoverListForm;
}

class DiscoverListForm : public QWidget
{
	Q_OBJECT

public:
    explicit DiscoverListForm(QWidget *parent = 0);
	~DiscoverListForm();

signals:
	void signalDeviceSelected(const QString & id, const QString & ip);

public slots:
	void discoverUpdate(const QDateTime& timeStamp, const QString& name, const QString& address);

	void clear();

private slots:
	void on_tableView_doubleClicked(const QModelIndex &index);

private:
	Ui::DiscoverListForm *ui;

	QStandardItemModel m_discoverRecord;
	QSortFilterProxyModel m_discoverProxy;

};

#endif // DISCOVERLISTFORM_H
