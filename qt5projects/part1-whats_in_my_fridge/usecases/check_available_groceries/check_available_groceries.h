#ifndef CHECK_AVAILABLE_GROCERIES_H
#define CHECK_AVAILABLE_GROCERIES_H

#include <QObject>

namespace entities {
	class GroceryItems;
}

namespace usecases {

class CheckAvailableGroceries : public QObject
{
Q_OBJECT
public:
	explicit CheckAvailableGroceries(entities::GroceryItems* groceryItems, QObject *parent = nullptr);

	void run();

signals:
	void failure(QString message);
	void success(QString message);

public slots:
	void onGroceryItemsAllRetrieved();
	void onGroceryItemsAllNotRetrieved();

private:
	entities::GroceryItems* m_groceryItems;

};


} // namespace usecases


#endif // CHECKAVAILABLEGROCERIES_H
