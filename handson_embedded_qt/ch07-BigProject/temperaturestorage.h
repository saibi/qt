#ifndef TEMPERATURESTORAGE_H
#define TEMPERATURESTORAGE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "temperaturereading.h"


class TemperatureStorage
{
public:
	TemperatureStorage();
	~TemperatureStorage();

	void addReading(const TemperatureReading &reading);
	QList<TemperatureReading> getAllReadings() const;
	QList<TemperatureReading> getLastNReadings(int n) const;
	TemperatureReading getLastReading() const;

	void clearAllReadings();
	int numberOfReadings() const;

	bool openStorage(const QString &id = "readings");
	void closeStorage();

	QString id() const;

	QSqlQueryModel *temperatureModel() const;

private:
	QString m_id;
	QSqlDatabase m_db;

	QSqlQueryModel * m_temperatureModel;

};

#endif // TEMPERATURESTORAGE_H
