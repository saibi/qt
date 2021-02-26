#include <QString>
#include <QtTest>

class Uc_broadcaster_connects_to_sensorTest : public QObject
{
	Q_OBJECT

public:
	Uc_broadcaster_connects_to_sensorTest();

private Q_SLOTS:
	void testCase1();
};

Uc_broadcaster_connects_to_sensorTest::Uc_broadcaster_connects_to_sensorTest()
{
}

void Uc_broadcaster_connects_to_sensorTest::testCase1()
{
	QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Uc_broadcaster_connects_to_sensorTest)

#include "tst_uc_broadcaster_connects_to_sensortest.moc"
