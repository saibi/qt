#include "hvaccontroller.h"
#include <QDebug>

HVACController::HVACController(QObject *parent) : QObject(parent)
{

}

void HVACController::setHeatingOn(bool on)
{
	qDebug() << Q_FUNC_INFO << on;
}

void HVACController::setCoolingOn(bool on)
{
	qDebug() << Q_FUNC_INFO << on;
}

void HVACController::setFanOn(bool on)
{
	qDebug() << Q_FUNC_INFO << on;
}
