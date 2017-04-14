#include "sdobserver2.h"

#include <QMutexLocker>

#include <QTime>
#include "motorhandler.h"

#define SD_CHECK_MSEC 1000

SdObserver2::SdObserver2(QObject *parent) :
    QThread(parent),
	_mutex(QMutex::NonRecursive)
{
    _stopFlag = false;
}

bool SdObserver2::startObserver()
{
	qDebug("[SdObserver2::startObserver] Start.");

    if (this->isRunning())
    {
        stopObserver();
    }

	QMutexLocker locker(&_mutex);

	_stopFlag = false;

    start();

	qDebug("[SdObserver2::startObserver] Done.");

    return true;
}

void SdObserver2::stopObserver()
{
	qDebug("[SdObserver2::stopObserver] Start.");
    QMutexLocker locker(&_mutex);

    _stopFlag = true;

	locker.unlock();

	qDebug("[SdObserver2::stopObserver] wait");

    wait();

	qDebug("[SdObserver2::stopObserver] end");
}

void SdObserver2::run()
{
	qDebug("[SdObserver2::run] Start.");

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

		if ( QTime::currentTime().second() % 10 == 0)
			MotorHandler::instance().hello("observer2");


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
				qDebug("[SdObserver2::run] SD removed.");

                // removed
                emit signalSdUnmounted();

                _mountFlag = false;
            }

            _sdFlag = sdFlag;
        }
        this->msleep(SD_CHECK_MSEC);
    }

	qDebug("[SdObserver2::run] Stop.");
}

bool SdObserver2::sdExists()
{
	//qDebug("[SdObserver2::sdExists] Start.");

	if ( QTime::currentTime().second() < 2 )
		return false;
	else if ( QTime::currentTime().second() < 5 )
		return true;
	else if ( QTime::currentTime().second() < 7 )
		return false;
	else if ( QTime::currentTime().second() < 10 )
		return true;
	else if ( QTime::currentTime().second() < 11 )
		return false;
	else if ( QTime::currentTime().second() < 13 )
		return true;
	else if ( QTime::currentTime().second() < 17 )
		return false;
	else if ( QTime::currentTime().second() < 21 )
		return true;
	else if ( QTime::currentTime().second() < 23 )
		return false;
	else if ( QTime::currentTime().second() < 25 )
		return true;
	else if ( QTime::currentTime().second() < 27 )
		return false;
	else if ( QTime::currentTime().second() < 30 )
		return true;
	else if ( QTime::currentTime().second() < 31 )
		return false;
	else if ( QTime::currentTime().second() < 39 )
		return true;
	else if ( QTime::currentTime().second() < 41 )
		return false;
	else if ( QTime::currentTime().second() < 43 )
		return true;
	else if ( QTime::currentTime().second() < 47 )
		return false;
	else if ( QTime::currentTime().second() < 50 )
		return true;
	else if ( QTime::currentTime().second() < 55 )
		return false;


	return true;
}
