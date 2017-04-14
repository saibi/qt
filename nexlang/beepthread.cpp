#include "beepthread.h"

#include <QMutexLocker>

BeepThread::BeepThread(QObject *parent) :
	QThread(parent)
{
}

void BeepThread::stop()
{
	QMutexLocker locker(&_mutex);

	_stopFlag = true;

	locker.unlock();

	_waitCondition.wakeOne();

	wait();
}

void BeepThread::beep(unsigned int beepMsecs, unsigned int muteMsecs)
{
	QMutexLocker locker(&_mutex);

	beep_i(beepMsecs, muteMsecs);

	_waitCondition.wakeOne();
}

void BeepThread::beep(unsigned int beep1Msecs, unsigned int mute1Msecs, unsigned int beep2Msecs, unsigned int mute2Msecs)
{
	QMutexLocker locker(&_mutex);

	beep_i(beep1Msecs, mute1Msecs);
	beep_i(beep2Msecs, mute2Msecs);

	_waitCondition.wakeOne();
}

void BeepThread::beep(unsigned int beep1Msecs, unsigned int mute1Msecs, unsigned int beep2Msecs, unsigned int mute2Msecs, unsigned int beep3Msecs, unsigned int mute3Msecs)
{
	QMutexLocker locker(&_mutex);

	beep_i(beep1Msecs, mute1Msecs);
	beep_i(beep2Msecs, mute2Msecs);
	beep_i(beep3Msecs, mute3Msecs);

	_waitCondition.wakeOne();
}

void BeepThread::beep_i(unsigned int beepMsecs, unsigned int muteMsecs)
{
	_beepMuteQueue.enqueue(beepMsecs);
	_beepMuteQueue.enqueue(muteMsecs);
}

void BeepThread::run()
{
	unsigned int beepMsecs;
	unsigned int muteMsecs;

	QMutexLocker locker(&_mutex);

	_stopFlag = false;

	while (1)
	{
		if (_stopFlag)
			break;

		while (!_beepMuteQueue.isEmpty())
		{
			// dequeue
			beepMsecs = _beepMuteQueue.dequeue();

			if (_beepMuteQueue.isEmpty())
				muteMsecs = 0;
			else
				muteMsecs = _beepMuteQueue.dequeue();

#ifdef __linux__
			if (_fdBeep)
				::write(_fdBeep, "!", 1);
#endif

			msleep(beepMsecs);

#ifdef __linux__
			if (_fdBeep)
				::write(_fdBeep, " ", 1);
#endif

			msleep(muteMsecs);
		}

		if (_waitCondition.wait(locker.mutex(), 100))
		{
			while (!_beepMuteQueue.isEmpty())
			{
				// dequeue
				beepMsecs = _beepMuteQueue.dequeue();

				if (_beepMuteQueue.isEmpty())
					muteMsecs = 0;
				else
					muteMsecs = _beepMuteQueue.dequeue();

#ifdef __linux__
				if (_fdBeep)
					::write(_fdBeep, "!", 1);
#endif

				msleep(beepMsecs);

#ifdef __linux__
				if (_fdBeep)
					::write(_fdBeep, " ", 1);
#endif

				msleep(muteMsecs);
			}
		}
		else
		{
			// timeout
#ifdef __linux__
			if (_fdBeep)
				::write(_fdBeep, " ", 1);
#endif
		}
	}
}
