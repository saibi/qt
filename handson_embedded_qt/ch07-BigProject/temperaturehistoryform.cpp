#include "temperaturehistoryform.h"
#include "ui_temperaturehistoryform.h"

#include "temperaturestorage.h"
#include <QQmlEngine>
#include <QQmlContext>

TemperatureHistoryForm::TemperatureHistoryForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TemperatureHistoryForm)
{
	ui->setupUi(this);

	m_tempProxy.setSourceModel(&m_tempRecord);
	ui->tableView->setModel(&m_tempProxy);

	ui->quickWidget->resize(QSize(640, 480));
	ui->quickWidget->engine()->rootContext()->setContextProperty("temperatureData", this);
	ui->quickWidget->setSource(QUrl("qrc:/TemperatureChart.qml"));
	ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

}

TemperatureHistoryForm::~TemperatureHistoryForm()
{
	delete ui;
}

void TemperatureHistoryForm::temperatureUpdate(QDateTime timeStamp, float temperature)
{
	auto time = new QStandardItem;
	auto temp = new QStandardItem;

	time->setData(timeStamp, Qt::DisplayRole);
	temp->setData(temperature, Qt::DisplayRole);


	QList <QStandardItem *> row;
	row << time << temp;

	m_tempRecord.appendRow(row);

	double xVal = timeStamp.toSecsSinceEpoch();
	setLastReading(QPointF(xVal, temperature));
}

QPointF TemperatureHistoryForm::lastReading() const
{
	return m_lastReading;
}

void TemperatureHistoryForm::setLastReading(const QPointF &lastReading)
{
	m_lastReading = lastReading;
	emit newReading();
}

TemperatureStorage *TemperatureHistoryForm::temperatureStorage() const
{
	return m_temperatureStorage;
}

void TemperatureHistoryForm::setTemperatureStorage(TemperatureStorage *temperatureStorage)
{
	m_temperatureStorage = temperatureStorage;
	m_tempProxy.setSourceModel(m_temperatureStorage->temperatureModel());
}
