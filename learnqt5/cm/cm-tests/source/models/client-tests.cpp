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
	void testCase2();
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

void ClientTests::testCase2()
{
	QVERIFY2(false, "Failure");
}

//QTEST_APPLESS_MAIN(ClientTests)

#include "client-tests.moc"
