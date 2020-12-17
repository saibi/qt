#include "remove_grocery_item.h"
#include "../../entities/grocery_items.h"

namespace usecases {

RemoveGroceryItem::RemoveGroceryItem(entities::GroceryItems *groceryItems, QObject *parent) : QObject(parent), m_groceryItems(groceryItems)
{
	connect(m_groceryItems, &entities::GroceryItems::removed, this, &RemoveGroceryItem::onGroceryItemRemoved);
	connect(m_groceryItems, &entities::GroceryItems::notRemoved, this, &RemoveGroceryItem::onGroceryItemNotRemoved);
}

void RemoveGroceryItem::run(const QString &name)
{
	m_groceryItems->remove(name);
}

void RemoveGroceryItem::onGroceryItemRemoved()
{
	emit success("REMOVE_GROCERY_ITEM__SUCCESS");
}

void RemoveGroceryItem::onGroceryItemNotRemoved()
{
	emit failure("REMOVE_GROCERY_ITEM__FAILURE");
}

} // namespace
