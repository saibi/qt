#include <QString>
#include <QtTest>
#include <QCoreApplication>

class Usecases_check_available_groceries : public QObject
{
	Q_OBJECT

public:
	Usecases_check_available_groceries();

private Q_SLOTS:
	void one_or_more_grocery_items_available();
};

Usecases_check_available_groceries::Usecases_check_available_groceries()
{
}

void Usecases_check_available_groceries::one_or_more_grocery_items_available()
{
	// Given there is a list of available grocery items
	QVERIFY2(false, "not implemented");
//	auto groceryItems = new entities::GroceryItems(this);
//	QVERIFY(groceryItems);


	// And one or more grocery items are actually available
	QFAIL("not implemented");
//	auto groceryItemsRepoDummy = new repositories::GroceryItemsRepoDummy(groceryItems);
//	groceryItems->setRepository(groceryItemsRepoDummy);
//	QVERIFY(groceryItemsRepoDummy->count() > 0);


	// When I check available groceries
	QFAIL("not implemented");
//	auto checkAvailableGroceries = new usecases::CheckAvailableGroceries(groceryItems, this);
//	QSignalSpy checkAvailableGroceriesSuccess(checkAvailableGroceries, &usecases::CheckAvailableGroceries::success);
//	checkAvailableGroceries->run();
//	QTRY_COMPARE_WITH_TIMEOUT(checkAvailableGroceriesSuccess.count(), 1, 1000);

	// Then I am given the list of available grocery items
	QVERIFY2(false, "not implemented");
//	QCOMPARE(groceryItems->list().count(), groceryItemsRepoDummy->count());

	// And the grocery items are ordered by name, ascending
	QVERIFY2(false, "not implemented");
//	QVERIFY(groceryItems->isSortedBy("name","ASC"));

}

QTEST_MAIN(Usecases_check_available_groceries)

#include "tst_usecases_check_available_groceries.moc"
