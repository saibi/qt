#include "sdobserver3.h"

#include <QMutexLocker>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>

#define SD_CHECK_MSEC 1000

SdObserver3::SdObserver3(QObject *parent) :
    QThread(parent),
    _mutex(QMutex::Recursive)
{
    _stopFlag = false;
}

bool SdObserver3::startObserver()
{
	qDebug("[SdObserver3::startObserver] Start.");

    if (this->isRunning())
    {
        stopObserver();
    }

	QMutexLocker locker(&_mutex);

	_stopFlag = false;

    start();

	qDebug("[SdObserver3::startObserver] Done.");

    return true;
}

void SdObserver3::stopObserver()
{
    QMutexLocker locker(&_mutex);

    _stopFlag = true;

    locker.unlock();

    wait();
}

void SdObserver3::run()
{
	qDebug("[SdObserver3::run] Start.");

    // check already mounted sd card & umount
    // TODO

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
                // inserted
				emit signalSdMounted();

                _mountFlag = true;
            }
            else
            {
				qDebug("[SdObserver3::run] SD removed.");

                // removed
                emit signalSdUnmounted();

                _mountFlag = false;
            }

            _sdFlag = sdFlag;
        }
        this->msleep(SD_CHECK_MSEC);
    }

	qDebug("[SdObserver3::run] Stop.");
}

#include <QTime>

bool SdObserver3::sdExists()
{
	//qDebug("[SdObserver3::sdExists] Start.");

	if ( QTime::currentTime().second() % 2  )
		return true;

	return false;
}
