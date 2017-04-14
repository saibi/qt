#include "backlight.h"

#include <QMutex>
#include <QMutexLocker>

#ifdef __arm__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#endif

#define BACKLIGHT_DEVICE    "/dev/backlight" // Backlight Device Name

static QMutex g_mutex;

int Backlight::_fdBacklight = 0;

Backlight::Backlight()
{
}

int Backlight::init()
{
	QMutexLocker locker(&g_mutex);

	return init_i();
}

int Backlight::init_i()
{
#ifdef __arm__
	if (_fdBacklight)
		return 0;

	_fdBacklight = ::open(BACKLIGHT_DEVICE, O_RDWR);
	if (_fdBacklight < 0) {
		qDebug("Failed to open backlight device: %s", BACKLIGHT_DEVICE);
		return -1;
	}
#endif
	return 0;
}

int Backlight::close()
{
#ifdef __arm__
	QMutexLocker locker(&g_mutex);

	if (_fdBacklight) {
		::close(_fdBacklight);
		_fdBacklight = 0;
	}
#endif
	return 0;
}

int Backlight::control(const char* controlString)
{
#ifdef __arm__
	QMutexLocker locker(&g_mutex);

	if (_fdBacklight == 0) {
		init_i();
	}

	return (int) ::write(_fdBacklight, controlString, strlen(controlString));
#else
	return 0;
#endif
}
