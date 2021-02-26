#include <QString>
#include <QtTest>

class Entity_broadcasterTest : public QObject
{
	Q_OBJECT

public:
	Entity_broadcasterTest();

private Q_SLOTS:
	void testCase1();
};

Entity_broadcasterTest::Entity_broadcasterTest()
{
}

void Entity_broadcasterTest::testCase1()
{
	QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Entity_broadcasterTest)

#include "tst_entity_broadcastertest.moc"
