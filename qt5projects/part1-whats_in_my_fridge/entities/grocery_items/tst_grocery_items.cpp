#include <QString>
#include <QtTest>
#include <QCoreApplication>

class Entities_Groceryitems : public QObject
{
	Q_OBJECT

public:
	Entities_Groceryitems();

private Q_SLOTS:
	void testCase1();
};

Entities_Groceryitems::Entities_Groceryitems()
{
}

void Entities_Groceryitems::testCase1()
{
	QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Entities_Groceryitems)

#include "tst_grocery_items.moc"
