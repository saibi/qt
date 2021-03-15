#include "temperaturestorage.h"

#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>


TemperatureStorage::TemperatureStorage()
{

}

TemperatureStorage::~TemperatureStorage()
{

}
void TemperatureStorage::addReading(const TemperatureReading &reading)
{
	QSqlQuery query(m_db);
	query.prepare("insert into TemperatureHistory values (:timestamp, :temperature);");
	query.bindValue(":timestamp", reading.timeStamp());
	query.bindValue(":temperature", reading.temperatureC());
	query.exec();

	m_temperatureModel->setQuery("select * from TemperatureHistory order by timestamp desc;", m_db);
}

QList<TemperatureReading> TemperatureStorage::getAllReadings() const
{
	QList<TemperatureReading> data;


	QSqlTableModel model(nullptr, m_db);
	model.setTable("TemperatureHistory");
	model.select();

	for ( int row = 0; row < model.rowCount(); ++ row)
	{
		auto rec = model.record(row);
		data << TemperatureReading(rec.value("timestamp").toDateTime(), rec.value("temperature").toFloat());
	}

	return data;
}

QList<TemperatureReading> TemperatureStorage::getLastNReadings(int n) const
{
	QList<TemperatureReading> data;

	auto qStr = QString("select * from TemperatureHistory order by timestamp desc limit %1").arg(n);
	QSqlQuery query(qStr, m_db);
	query.exec();
	while(query.next())
	{
		data << TemperatureReading(query.value("timestamp").toDateTime(), query.value("temperature").toFloat());
	}

	return data;
}

TemperatureReading TemperatureStorage::getLastReading() const
{
	auto data = getLastNReadings(1);
	return data.value(0, TemperatureReading());
}

void TemperatureStorage::clearAllReadings()
{
	QSqlQuery query("delete from TemperatureHistory where 1;", m_db);
	query.exec();

	m_temperatureModel->query().exec();
}

int TemperatureStorage::numberOfReadings() const
{
	QSqlQuery query("select count(timestamp) from TemperatureHistory;", m_db);
	query.exec();
	query.next();
	return query.value(1).toInt();
}

bool TemperatureStorage::openStorage(const QString &id)
{
	QString dbName = QString("ReadingStore_%1").arg(id);
	QString dbFileName = QString("%1.sqlite").arg(dbName);

	m_db = QSqlDatabase::addDatabase("QSQLITE", dbName);
	m_db.setDatabaseName(dbFileName);

	bool result = m_db.open();

	if ( result)
	{
		QSqlQuery query(m_db);
		result = query.exec("create table if not exists TemperatureHistory ("
"timestamp datetime primary key, temperature float );" );

		m_temperatureModel = new QSqlQueryModel;
		m_temperatureModel->setQuery("select * from TemperatureHistory order by timestamp desc;", m_db);
	}

	if ( !result)
	{
		qDebug() << "database failure" << m_db.lastError();
	}
	return result;
}

void TemperatureStorage::closeStorage()
{
	m_db.close();
}

QString TemperatureStorage::id() const
{
	return m_id;
}

QSqlQueryModel *TemperatureStorage::temperatureModel() const
{
	return m_temperatureModel;
}
