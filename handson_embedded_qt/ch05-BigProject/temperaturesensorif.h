#ifndef TEMPERATURESENSORIF_H
#define TEMPERATURESENSORIF_H

#include <QObject>
#include <QDateTime>

class TemperatureSensorIF : public QObject
{
	Q_OBJECT
public:
	explicit TemperatureSensorIF(QObject *parent = nullptr);

signals:
	void newTemperature(QDateTime timeStamp, float degreesCelsius);


};

#endif // TEMPERATURESENSORIF_H
