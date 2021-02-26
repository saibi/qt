#include <QString>
#include <QtTest>

class Uc_broadcaster_publishes_sensor_readingsTest : public QObject
{
	Q_OBJECT

public:
	Uc_broadcaster_publishes_sensor_readingsTest();

private Q_SLOTS:
	void init();
	void cleanup();
	void test_sensor_emits_single();

private:
	entities::Broadcaster * _broadcaster;
	entities::Sensor * _sensor;
};

Uc_broadcaster_publishes_sensor_readingsTest::Uc_broadcaster_publishes_sensor_readingsTest()
{
}

void Uc_broadcaster_publishes_sensor_readingsTest::init()
{
	// Given there is a broadcaster
	_broadcaster = new entities::Broadcaster;
	QVERIFY(_broadcaster);

	// And there is a sensor
	_sensor = new entities::Sensor("mockSensor1");
	QVERIFY(_sensor);

	// And the broadcaster connects to the sensor
	QVERIFY(usecases::broadcaster_connects_to_sensor(*_broadcaster, *_sensor));
}

void Uc_broadcaster_publishes_sensor_readingsTest::cleanup()
{
	delete _broadcaster;
	_broadcaster = nullptr;

	delete _sensor;
	_sensor = nullptr
}

void Uc_broadcaster_publishes_sensor_readingsTest::test_sensor_emits_single()
{
	// And the sensor emits a reading
	QSignalSpy sensorEmitReading(_sensor, SIGNAL(emitReading(QVariantMap)));
	_sensor->emitReading(QVariantMap({
										 {"sensor_id", _sensor->identifier() },
										 {"timestamp", QDateTime::currentMSecsSinceEpoch()},
										 {"value", 1.5} } ));

	QCOMPARE(sensorEmitReading.count(), 1);

	// When the broadcaster publishes the sensor readings
	bool published_reading_coincides_with_sensor_readings = usecases::broadcaster_publishes_sensor_readings(*_broadcaster, *_sensor);


	// Then the corresponding published reading coincides with the sensor reading
	QVERIFY(published_reading_coincides_with_sensor_readings);
}

QTEST_APPLESS_MAIN(Uc_broadcaster_publishes_sensor_readingsTest)

#include "tst_uc_broadcaster_publishes_sensor_readingstest.moc"
