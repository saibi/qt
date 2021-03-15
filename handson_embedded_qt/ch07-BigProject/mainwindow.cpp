#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(TemperatureSensorIF *tempSensor, QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_tempSensor(tempSensor)
	, m_tempStorage(new TemperatureStorage)
{
	ui->setupUi(this);

	updateDisplay();

	connect(&m_updateTimer, &QTimer::timeout, this, &MainWindow::updateDisplay);

	m_tempStorage->openStorage("history");
	ui->historyForm->setTemperatureStorage(m_tempStorage);

	m_updateTimer.setInterval(1000);
	m_updateTimer.start();

	updateTempDisplay(QDateTime(), 0.0);
	connect(m_tempSensor, &TemperatureSensorIF::newTemperature, this, &MainWindow::updateTempDisplay);
	connect(m_tempSensor, &TemperatureSensorIF::newTemperature, ui->historyForm, &TemperatureHistoryForm::temperatureUpdate);
	connect(m_tempSensor, &TemperatureSensorIF::newTemperature, [this](QDateTime timeStamp, float temperature) {
		m_tempStorage->addReading(TemperatureReading(timeStamp, temperature));
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::updateDisplay()
{
	QDateTime now = QDateTime::currentDateTime();
	ui->currentDateTime->setText(now.toString());
}

void MainWindow::updateTempDisplay(QDateTime date, float temp)
{
	ui->tempDisplay->setText(QString::number(temp) + " C");
}
