#include <QString>
#include <QtTest>

class ClientTests : public QObject
{
	Q_OBJECT

public:
	ClientTests();

private Q_SLOTS:
	void initTestCase();
	void cleanupTestCase();
	void testCase1();
};

ClientTests::ClientTests()
{
}

void ClientTests::initTestCase()
{
}

void ClientTests::cleanupTestCase()
{
}

void ClientTests::testCase1()
{
	QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(ClientTests)

#include "client-tests.moc"
