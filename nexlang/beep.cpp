#include "beep.h"

#include <QMutex>
#include <QMutexLocker>

#include "beepthread.h"

#ifdef __arm__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#else
#include <QApplication>
#endif

#define BEEP_DEVICE    "/dev/beep" // Beep Device Name

static QMutex g_mutex;

static BeepThread* g_beepThread = 0;

int Beep::_fdBeep = 0;

bool Beep::_beepFlag = true;

Beep::Beep()
{
}

Beep::~Beep()
{
	close();
}

int Beep::init()
{
	QMutexLocker locker(&g_mutex);

	return init_i();
}

int Beep::init_i()
{
#ifdef __arm__
	if (_fdBeep)
		return 0;

	_fdBeep = ::open(BEEP_DEVICE, O_RDWR);

	if (_fdBeep < 0) {
		qDebug("Failed to open beep device: %s", BEEP_DEVICE);
		return -1;
	}

	g_beepThread = new BeepThread;

	if (g_beepThread)
	{
		g_beepThread->setData(_fdBeep);
		g_beepThread->start(QThread::HighPriority);
	}
#else
	g_beepThread = 0;
#endif
	return 0;
}

int Beep::close()
{
#ifdef __arm__
	QMutexLocker locker(&g_mutex);

	if (g_beepThread && g_beepThread->isRunning())
	{
		g_beepThread->stop();
	}

	if (g_beepThread)
	{
		delete g_beepThread;
		g_beepThread = 0;
	}

	if (_fdBeep) {
		::close(_fdBeep);
		_fdBeep = 0;
	}
#endif
	return 0;
}

void Beep::beep(unsigned int beepMsecs, unsigned int muteMsecs)
{
#ifdef __arm__
	QMutexLocker locker(&g_mutex);

	if (_fdBeep == 0) {
		init_i();
	}

	if (g_beepThread && g_beepThread->isRunning() && _beepFlag)
	{
		g_beepThread->beep(beepMsecs, muteMsecs);
	}
#else
	//QApplication::beep();
#endif
}

void Beep::beep(unsigned int beep1Msecs, unsigned int mute1Msecs, unsigned int beep2Msecs, unsigned int mute2Msecs)
{
#ifdef __arm__
	QMutexLocker locker(&g_mutex);

	if (_fdBeep == 0) {
		init_i();
	}

	if (g_beepThread && g_beepThread->isRunning() && _beepFlag)
	{
		g_beepThread->beep(beep1Msecs, mute1Msecs, beep2Msecs, mute2Msecs);
	}
#else
	//QApplication::beep();
#endif
}

void Beep::beep(unsigned int beep1Msecs, unsigned int mute1Msecs, unsigned int beep2Msecs, unsigned int mute2Msecs, unsigned int beep3Msecs, unsigned int mute3Msecs)
{
#ifdef __arm__
	QMutexLocker locker(&g_mutex);

	if (_fdBeep == 0) {
		init_i();
	}

	if (g_beepThread && g_beepThread->isRunning() && _beepFlag)
	{
		g_beepThread->beep(beep1Msecs, mute1Msecs, beep2Msecs, mute2Msecs, beep3Msecs, mute3Msecs);
	}
#else
	//QApplication::beep();
#endif
}

void Beep::setEnabled(bool flag)
{
	QMutexLocker locker(&g_mutex);

	_beepFlag = flag;
}
