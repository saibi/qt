#include "sdobserver.h"

#include <QMutexLocker>

#define SD_CHECK_MSEC 500

SdObserver::SdObserver(QObject *parent) :
    QThread(parent),
    _mutex(QMutex::Recursive)
{
    _stopFlag = false;
}

bool SdObserver::startObserver(const QString & id)
{
	qDebug("[SdObserver::startObserver] Start [%s]", qPrintable(id));

    if (this->isRunning())
    {
        stopObserver();
    }

	//
	QMutexLocker locker(&_mutex);

	m_id = id;
	_stopFlag = false;

    start();

    qDebug("[SdObserver::startObserver] Done.");

    return true;
}

void SdObserver::stopObserver()
{
    QMutexLocker locker(&_mutex);

    _stopFlag = true;

    locker.unlock();

    wait();
}

void SdObserver::run()
{
    qDebug("[SdObserver::run] Start.");

    //
    bool sdFlag;

    // reset flag
    _sdFlag = false;
    _mountFlag = false;

    while (1)
    {
        if (_stopFlag)
            break;

        sdFlag = sdExists();

        if (sdFlag != _sdFlag)
        {
            if (sdFlag)
            {
				qDebug("[SdObserver::run] mounted [%s]", qPrintable(m_id));

				emit signalSdMounted();

                _mountFlag = true;
            }
            else
            {
				qDebug("[SdObserver::run] unmounted [%s]", qPrintable(m_id));

                emit signalSdUnmounted();

                _mountFlag = false;
            }

            _sdFlag = sdFlag;
        }

        this->msleep(SD_CHECK_MSEC);
    }

    qDebug("[SdObserver::run] Stop.");
}

#include <QTime>

bool SdObserver::sdExists()
{
    //qDebug("[SdObserver::sdExists] Start.");

	if ( QTime::currentTime().second() < 10 )
		return true;
	else if ( QTime::currentTime().second() < 20 )
		return false;
	else if ( QTime::currentTime().second() < 30 )
		return true;
	else if ( QTime::currentTime().second() < 40 )
		return false;
	else if ( QTime::currentTime().second() < 50 )
		return true;

	return false;
}
