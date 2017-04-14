#include "configmanager.h"

#include <QDateTime>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent), _mutex(QMutex::NonRecursive)
{
	qsrand( QDateTime::currentDateTime().toMSecsSinceEpoch() );

	m_error = 0;
}

ConfigManager::~ConfigManager()
{
}

bool ConfigManager::reopen()
{
	QMutexLocker locker(&_mutex);

	qDebug("[ConfigManager::reopen]");

	return true;
}

bool ConfigManager::setData()
{
	QMutexLocker locker(&_mutex);

	qDebug("[ConfigManager::setData]");

	m_error = qrand() % 100;

	if ( m_error & 0x01 )
	{
		qDebug("[ConfigManager::setData] true");
		return true;
	}
	else
	{
		qDebug("[ConfigManager::setData] false");
		throw m_error;
	}
	qDebug("[ConfigManager::setData] end");

	return false;
}

bool ConfigManager::close()
{
	QMutexLocker locker(&_mutex);

	qDebug("[ConfigManager::close]");
	return true;
}
