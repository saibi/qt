#include "mocktempsensor.h"

#include <QDateTime>

MockTempSensor::MockTempSensor(QObject *parent) : TemperatureSensorIF(parent)
{
	m_lastTemp = 20.0;
	m_sendTempTimer.setInterval(4000);
	m_sendTempTimer.setSingleShot(false);
	connect(&m_sendTempTimer, &QTimer::timeout, this, [this]() {
		QDateTime now = QDateTime::currentDateTime();

		float change = static_cast<float>(now.time().second() % 5 - 2);
		m_lastTemp += change;
		emit newTemperature(now, m_lastTemp);
	});
	m_sendTempTimer.start();
}
