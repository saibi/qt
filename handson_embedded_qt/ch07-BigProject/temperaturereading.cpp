#include "temperaturereading.h"

TemperatureReading::TemperatureReading() : m_temperatureC(0.0), m_timeStamp(QDateTime())
{

}

TemperatureReading::TemperatureReading(const QDateTime &timeStamp, float temperature) : m_temperatureC(temperature), m_timeStamp(timeStamp)
{

}

bool TemperatureReading::operator==(const TemperatureReading &right)
{
	return ( (right.m_timeStamp == m_timeStamp) && (right.m_temperatureC == m_temperatureC));
}

bool TemperatureReading::operator<(const TemperatureReading &right)
{
	return ( m_timeStamp.toMSecsSinceEpoch() < right.m_timeStamp.toMSecsSinceEpoch());
}

float TemperatureReading::temperatureC() const
{
	return m_temperatureC;
}

void TemperatureReading::setTemperatureC(float temperatureC)
{
	m_temperatureC = temperatureC;
}

QDateTime TemperatureReading::timeStamp() const
{
	return m_timeStamp;
}

void TemperatureReading::setTimeStamp(const QDateTime &timeStamp)
{
	m_timeStamp = timeStamp;
}

bool TemperatureReading::isValid() const
{
	return m_timeStamp.isValid();
}
