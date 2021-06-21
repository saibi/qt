#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mocktempsensor.h"

MainWindow::MainWindow(TemperatureSensorIF *tempSensor, QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_tempSensor(tempSensor)
	, m_tempStorage(new TemperatureStorage)
{
	ui->setupUi(this);

	ui->label_heatingInd->setEnabled(false);
	ui->label_coolingInd->setEnabled(false);
	ui->label_fanInd->setEnabled(false);

	//m_hvacSM.moveToThread(new QThread);

	m_hvacSM.connectToState("Heating", ui->label_heatingInd, &QLabel::setEnabled);
	m_hvacSM.connectToState("Cooling", ui->label_coolingInd, &QLabel::setEnabled);
	//m_hvacSM.connectToState("FanOn", ui->label_fanInd, &QLabel::setEnabled);

	m_hvacSM.connectToState("Heating", &m_hvacCtrl, &HVACController::setHeatingOn);
	m_hvacSM.connectToState("Cooling", &m_hvacCtrl, &HVACController::setCoolingOn);
	//m_hvacSM.connectToState("FanOn", &m_hvacCtrl, &HVACController::setFanOn);

	m_hvacSM.start();


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
