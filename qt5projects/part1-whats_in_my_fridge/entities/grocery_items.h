#ifndef GROCERY_ITEMS_H
#define GROCERY_ITEMS_H

#include <QObject>
#include <QVariantList>

namespace repositories {
	class GroceryItemsRepo;
}

namespace entities {

class GroceryItems : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QVariantList list READ list NOTIFY listChanged)

public:
	explicit GroceryItems(QObject *parent = nullptr);
	~GroceryItems();
	void sortBy(const QString& field, const QString& direction);
	bool isSortedBy(const QString& field, const QString& direction) const;
	void setRepository(repositories::GroceryItemsRepo* repository);
	void retrieveAll();
	QVariantList list() const;

	void create(const QString & name);
	bool contains(const QString &field, const QString &value) const;
	void remove(const QString & name);

signals:
	void allRetrieved(QString message);
	void allNotRetrieved(QString message);
	void listChanged();
	void created(QString message);
	void notCreated(QString message);
	void removed(QString message);
	void notRemoved(QString message);

private:
	bool m_isSortedByNameAsc;
	QVariantList m_list;
	repositories::GroceryItemsRepo* m_repository;


private slots:
	void onAllRecordsRetrieved(const QVariantList& records);
	void onAllRecordsNotRetrieved(QString message) {}
};


} // namespace entities
#endif // GROCERY_ITEMS_H
