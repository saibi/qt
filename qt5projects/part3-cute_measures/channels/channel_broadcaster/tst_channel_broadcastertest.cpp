#include <QString>
#include <QtTest>

#include "broadcasterbt.h"
#include "../../entities/entity_broadcaster/broadcaster.h"


class Channel_broadcasterTest : public QObject
{
	Q_OBJECT

public:
	Channel_broadcasterTest();

private Q_SLOTS:
	void init();
	void cleanup();
	void test_connectToBroadcaster();

private:
	channels::BroadcasterBt* _broadcaster_bt;
};

Channel_broadcasterTest::Channel_broadcasterTest()
{
}

void Channel_broadcasterTest::init()
{
	_broadcaster_bt = new channels::BroadcasterBt(this);
	QVERIFY(_broadcaster_bt->init());
}

void Channel_broadcasterTest::cleanup()
{
	delete _broadcaster_bt;
}

void Channel_broadcasterTest::test_connectToBroadcaster()
{
	auto broadcaster_entity = new entities::Broadcaster();
	QVERIFY(_broadcaster_bt->connectToBroadcaster(broadcaster_entity));
	delete broadcaster_entity;
}

QTEST_APPLESS_MAIN(Channel_broadcasterTest)

#include "tst_channel_broadcastertest.moc"
