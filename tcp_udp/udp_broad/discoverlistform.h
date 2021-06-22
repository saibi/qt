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
	explicit DiscoverListForm(QWidget *parent = nullptr);
	~DiscoverListForm();

public slots:
	void discoverUpdate(const QDateTime& timeStamp, const QString& name, const QString& address);

	void clear();

private:
	Ui::DiscoverListForm *ui;

	QStandardItemModel m_discoverRecord;
	QSortFilterProxyModel m_discoverProxy;

};

#endif // DISCOVERLISTFORM_H
