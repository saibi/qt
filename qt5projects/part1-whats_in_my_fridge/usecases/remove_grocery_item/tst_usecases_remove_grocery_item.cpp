#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../entities/grocery_items.h"
#include "../../repositories/grocery_items_repo_dummy.h"
#include "../check_available_groceries/check_available_groceries.h"
#include "../remove_grocery_item/remove_grocery_item.h"

class Usecases_remove_grocery_item : public QObject
{
	Q_OBJECT

public:
	Usecases_remove_grocery_item();

private Q_SLOTS:
	void init();
	void item_can_be_removed();
	void cleanup();

private:
	entities::GroceryItems *m_groceryItems;
};

Usecases_remove_grocery_item::Usecases_remove_grocery_item()
{
}

void Usecases_remove_grocery_item::init()
{
	qDebug() << "init";
	m_groceryItems = new entities::GroceryItems(this);
	auto groceryItemsRepoDummy = new repositories::GroceryItemsRepoDummy(this);
	m_groceryItems->setRepository(groceryItemsRepoDummy);
}

void Usecases_remove_grocery_item::item_can_be_removed()
{
	// Given I am given a list of available groceries
	//QVERIFY2(false, "Failure");
	auto checkAvailableGroceries = new usecases::CheckAvailableGroceries(m_groceryItems, this);
	QSignalSpy checkAvailableGroceriesSuccess(checkAvailableGroceries, &usecases::CheckAvailableGroceries::success);
	checkAvailableGroceries->run();
	QTRY_COMPARE_WITH_TIMEOUT(checkAvailableGroceriesSuccess.count(), 1, 1000);
	delete checkAvailableGroceries;

	// And the grocery item of name X is contained in the list
	QVERIFY(m_groceryItems->contains("name", "bananas"));

	// When I remove the grocery item with name X
	auto removeGroceryItem = new usecases::RemoveGroceryItem(m_groceryItems, this);
	QSignalSpy removeGroceryItemSuccess(removeGroceryItem, &usecases::RemoveGroceryItem::success);
	removeGroceryItem->run("bananas");
	QTRY_COMPARE_WITH_TIMEOUT(removeGroceryItemSuccess.count(), 1, 1000);
	delete removeGroceryItem;

	// Then the grocery item with name X is not contained in the list
	QVERIFY(!m_groceryItems->contains("name", "bananas"));

	// And the groceries are ordered by type, ascending
	QVERIFY(m_groceryItems->isSortedBy("name", "ASC"));
}


void Usecases_remove_grocery_item::cleanup()
{
	qDebug() << "cleanup";
	delete m_groceryItems;
}

QTEST_MAIN(Usecases_remove_grocery_item)

#include "tst_usecases_remove_grocery_item.moc"
