#ifndef BROADCASTER_CONNECTS_TO_SENSOR_H
#define BROADCASTER_CONNECTS_TO_SENSOR_H

namespace entities {
class Broadcaster;
class Sensor;
}

namespace usecases {
bool broadcaster_connects_to_sensor(const entities::Broadcaster & broadcaster, const entities::Sensor & sensor);
}

#endif // BROADCASTER_CONNECTS_TO_SENSOR_H
