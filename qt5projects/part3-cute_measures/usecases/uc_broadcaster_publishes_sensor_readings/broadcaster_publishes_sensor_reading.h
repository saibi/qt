#ifndef BROADCASTER_PUBLISHES_SENSOR_READING_H
#define BROADCASTER_PUBLISHES_SENSOR_READING_H

namespace entities {
class Sensor;
class Broadcaster;
}

namespace usecases {
bool broadcaster_publishes_sensor_readings(entities::Broadcaster & broadcaster, entities::Sensor & sensor);
}

#endif // BROADCASTER_PUBLISHES_SENSOR_READING_H
