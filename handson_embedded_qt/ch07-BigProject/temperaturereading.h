#ifndef TEMPERATUREREADING_H
#define TEMPERATUREREADING_H

#include <QDateTime>



class TemperatureReading
{
public:
	TemperatureReading();
	TemperatureReading(const QDateTime &timeStamp, float temperature);
	TemperatureReading &operator=(const TemperatureReading &right) = default;
	~TemperatureReading() = default;

	bool operator==(const TemperatureReading &right);
	bool operator<(const TemperatureReading &right);

	float temperatureC() const;
	void setTemperatureC(float temperatureC);

	QDateTime timeStamp() const;
	void setTimeStamp(const QDateTime &timeStamp);

	bool isValid() const;

private:
	float m_temperatureC;
	QDateTime m_timeStamp;
};

#endif // TEMPERATUREREADING_H
