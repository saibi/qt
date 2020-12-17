#include <QObject>
#include "check_available_groceries.h"
#include "../../entities/grocery_items.h"

namespace usecases {

CheckAvailableGroceries::CheckAvailableGroceries(entities::GroceryItems *groceryItems, QObject *parent)
	: QObject(parent),
	  m_groceryItems(groceryItems)
{

	connect(groceryItems, &entities::GroceryItems::allRetrieved, this, &CheckAvailableGroceries::onGroceryItemsAllRetrieved);
	connect(groceryItems, &entities::GroceryItems::allNotRetrieved, this, &CheckAvailableGroceries::onGroceryItemsAllNotRetrieved);
}

void CheckAvailableGroceries::run()
{
	m_groceryItems->retrieveAll();
}

void CheckAvailableGroceries::onGroceryItemsAllRetrieved() {
	emit success("CHECK_AVAILABLE_GROCERIES__SUCCESS");
}

void CheckAvailableGroceries::onGroceryItemsAllNotRetrieved() {
	emit failure("CHECK_AVAILABLE_GROCERIES__FAILURE");
}


} // namespace usecases
