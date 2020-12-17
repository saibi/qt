#ifndef REMOVE_GROCERY_ITEM_H
#define REMOVE_GROCERY_ITEM_H

#include <QObject>

namespace entities {
	class GroceryItems;
}

namespace usecases {

class RemoveGroceryItem : public QObject
{
	Q_OBJECT
public:
	explicit RemoveGroceryItem(entities::GroceryItems * groceryItems, QObject *parent= nullptr);

	Q_INVOKABLE void run(const QString & name);

signals:
	void success(QString message);
	void failure(QString message);

private slots:
	void onGroceryItemRemoved();
	void onGroceryItemNotRemoved();

private:
	entities::GroceryItems * m_groceryItems;
};

} // namespace

#endif // REMOVE_GROCERY_ITEM_H
