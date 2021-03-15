#ifndef TEMPERATUREHISTORYFORM_H
#define TEMPERATUREHISTORYFORM_H

#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QWidget>

class TemperatureStorage;

namespace Ui {
class TemperatureHistoryForm;
}

class TemperatureHistoryForm : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(QPointF lastReading READ lastReading WRITE setLastReading NOTIFY newReading)

public:
	explicit TemperatureHistoryForm(QWidget *parent = nullptr);
	~TemperatureHistoryForm();

	QPointF lastReading() const;
	void setLastReading(const QPointF &lastReading);

	TemperatureStorage *temperatureStorage() const;
	void setTemperatureStorage(TemperatureStorage *temperatureStorage);

public slots:
	void temperatureUpdate(QDateTime timeStamp, float temperature);

signals:
	void newReading();

private:
	Ui::TemperatureHistoryForm *ui;

	QStandardItemModel m_tempRecord;
	QSortFilterProxyModel m_tempProxy;

	QPointF m_lastReading;

	TemperatureStorage *m_temperatureStorage;
};

#endif // TEMPERATUREHISTORYFORM_H
