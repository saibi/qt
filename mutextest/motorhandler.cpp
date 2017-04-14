#include "motorhandler.h"
#include "notification.h"
#include "dataconverter.h"
#include <QApplication>

MotorHandler::MotorHandler(QObject *parent) :
	QObject(parent),
	m_mutex(QMutex::NonRecursive)
{
}

MotorHandler::~MotorHandler()
{
}


void MotorHandler::test(const QString & msg)
{
	QMutexLocker locker(&m_mutex);

	qDebug("[MotorHandler::test] start [%s]", qPrintable(msg));

	Notification::instance().notify("start");

	for (int i = 0 ; i < 100; i++)
	{
		qDebug("[%s] #%d prime = %d", qPrintable(msg), i, DataConverter::countPrimes(100000));
		QApplication::processEvents();
	}


	Notification::instance().notify("end");
	qDebug("[MotorHandler::test] end [%s]", qPrintable(msg));
}


void MotorHandler::hello(const QString & msg)
{
	qDebug("[MotorHandler::hello] start [%s]", qPrintable(msg));

	QMutexLocker locker(&m_mutex);

	for (int i = 0 ; i < 10000; i++)
	{
		DataConverter::countPrimes(10000);
	}
	qDebug("[MotorHandler::hello] end [%s]", qPrintable(msg));
}
