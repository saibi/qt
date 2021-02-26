#include <QString>
#include <QtTest>

class Entity_sensorTest : public QObject
{
	Q_OBJECT

public:
	Entity_sensorTest();

private Q_SLOTS:
	void testCase1();
};

Entity_sensorTest::Entity_sensorTest()
{
}

void Entity_sensorTest::testCase1()
{
	QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Entity_sensorTest)

#include "tst_entity_sensortest.moc"
