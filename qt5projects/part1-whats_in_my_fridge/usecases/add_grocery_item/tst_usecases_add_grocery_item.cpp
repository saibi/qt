#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../entities/grocery_items.h"
#include "../../repositories/grocery_items_repo_dummy.h"
#include "../check_available_groceries/check_available_groceries.h"
#include "../add_grocery_item/add_grocery_item.h"

class Usecases_add_grocery_item : public QObject
{
	Q_OBJECT

public:
	Usecases_add_grocery_item();

private Q_SLOTS:
	void init();
	void item_can_be_added();
	void cleanup();

private:
	entities::GroceryItems * m_groceryItems;
};

Usecases_add_grocery_item::Usecases_add_grocery_item()
{
}

void Usecases_add_grocery_item::init()
{
	qDebug() << "init";
	m_groceryItems = new entities::GroceryItems(this);
	auto groceryItemsRepoDummy = new repositories::GroceryItemsRepoDummy(this);
	m_groceryItems->setRepository(groceryItemsRepoDummy);
}

void Usecases_add_grocery_item::item_can_be_added()
{
	// Given I am given a list of available groceries
	//QVERIFY2(false, "not implemented");
	auto checkAvailableGroceries = new usecases::CheckAvailableGroceries(m_groceryItems, this);
	QSignalSpy checkAvailableGroceriesSuccess(checkAvailableGroceries, &usecases::CheckAvailableGroceries::success);
	checkAvailableGroceries->run();
	QTRY_COMPARE_WITH_TIMEOUT(checkAvailableGroceriesSuccess.count(), 1, 1000);
	delete checkAvailableGroceries;

	// When I add a grocery item with name X
	//QFAIL("not implemented");
	auto addGroceryItem = new usecases::AddGroceryItem(m_groceryItems, this);
	QSignalSpy addGroceryItemSuccess(addGroceryItem, &usecases::AddGroceryItem::success);
	addGroceryItem->run("avocados");
	QTRY_COMPARE_WITH_TIMEOUT(addGroceryItemSuccess.count(), 1, 1000);
	delete addGroceryItem;

	// Then the grocery item with name X is contained in the list
	//QFAIL("not implemented");
	QVERIFY(m_groceryItems->contains("name", "avocados"));

	// And the groceries are odrered by name, ascending
	//QFAIL("not implemented");
	QVERIFY(m_groceryItems->isSortedBy("name", "ASC"));
}

void Usecases_add_grocery_item::cleanup()
{
	qDebug() << "cleanup";
	delete m_groceryItems;
}

QTEST_MAIN(Usecases_add_grocery_item)

#include "tst_usecases_add_grocery_item.moc"
