#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "temperaturesensorif.h"
#include "temperaturestorage.h"
#include "hvaccontroller.h"
#include "HVACControl.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(TemperatureSensorIF *tempSensor, QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void updateDisplay();
	void updateTempDisplay(QDateTime date, float temp);

private:
	Ui::MainWindow *ui;

	QTimer m_updateTimer;
	TemperatureSensorIF *m_tempSensor;
	TemperatureStorage *m_tempStorage;

	HVACControl m_hvacSM;
	HVACController m_hvacCtrl;
};
#endif // MAINWINDOW_H
