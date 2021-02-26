#include <QtGlobal>

//#include "../../entities/entity_broadcaster/broadcaster.h"
//#include "../../entities/entity_sensor/sensor.h"

#include "broadcaster_publishes_sensor_reading.h"

bool usecases::broadcaster_publishes_sensor_reading(entities::Broadcaster& broadcaster, entities::Sensor& sensor)
{
	quint64 broadcasterTimestamp = broadcaster.lastPublishedReadings().at(0).value("timestamp").toUInt();
	quint64 sensorTimeStamp = sensor.lastReading().value("timestamp").toUInt();

	return broadcasterTimestamp > 0 && sensorTimeStamp > 0 && broadcasterTimestamp == sensorTimeStamp;
}
