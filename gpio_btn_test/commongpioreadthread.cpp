#include "commongpioreadthread.h"

#ifdef __linux__
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#endif

#include <QKeyEvent>
#include <QCoreApplication>

#ifdef __linux__

// linux uinput : emulate keyboard input from userspace

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <linux/uinput.h>

static int open_uinput(void)
{
	int fd = 0;
	struct uinput_user_dev uud;

	fd = ::open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if ( fd < 0 )
		return -1;

	if ( ::ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0 ||
		 ::ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0 ||
		 ::ioctl(fd, UI_SET_KEYBIT, KEY_F1) < 0 ||
		 ::ioctl(fd, UI_SET_KEYBIT, KEY_F12) < 0 ) // set keyboard code here ...
	{
		::close(fd);
		return -2;
	}

	::memset(&uud, 0, sizeof(uud));
	::snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "uinput-keyboard");
	uud.id.bustype = BUS_HOST;
	uud.id.vendor = 0xc8;
	uud.id.product = 0xc8;
	uud.id.version = 1;

	if ( ::write(fd, &uud, sizeof(uud)) > 0 && ::ioctl(fd, UI_DEV_CREATE) < 0 )
	{
		::close(fd);
		return -3;
	}

	return fd;
}

static int emit_code(int fd, int type, int code, int val)
{
	struct input_event ie;

	ie.type = static_cast<unsigned short>(type);
	ie.code = static_cast<unsigned short>(code);
	ie.value = val;
	::gettimeofday(&ie.time, NULL);

	if ( ::write(fd, &ie, sizeof(ie)) < 0 )
		return -1;

	return 0;
}

#if 0
static void send_keyboard_event(int fd, int key, int val)
{
	emit_code(fd, EV_KEY, key, val);
	emit_code(fd, EV_SYN, SYN_REPORT, val);
}
#endif

static void press_keyboard_once(int fd, int key)
{
	emit_code(fd, EV_KEY, key, 1);
	emit_code(fd, EV_SYN, SYN_REPORT, 1);
	emit_code(fd, EV_KEY, key, 0);
	emit_code(fd, EV_SYN, SYN_REPORT, 0);
}

static void close_uinput(int fd)
{
	if ( fd >= 0 )
	{
		::ioctl(fd, UI_DEV_DESTROY);
		::close(fd);
	}
}

static int convQtKey2uinput(int key)
{
	switch(key)
	{
	case Qt::Key_F1:
		key = KEY_F1;
		break;

	case Qt::Key_F12:
		key = KEY_F12;
		break;

	default:
		::fprintf(stderr, "WARNING - missing qt key, plz append key code [%d]", key);
		break;
	}
	return key;
}
#endif

CommonGpioReadThread::CommonGpioReadThread(QObject *parent) : QThread(parent)
{
	m_stopFlag = false;
	m_fd_uinput = -1;

#ifdef __linux__
	m_fd_uinput = open_uinput();
	if ( m_fd_uinput < 0 )
		qDebug("[CommonGpioReadThread] uinput is not available (%d)", m_fd_uinput);
	else
		qDebug("[CommonGpioReadThread] uinput available");
#endif
}

CommonGpioReadThread::~CommonGpioReadThread()
{
	closeDeviceFiles();
#ifdef __linux__
	close_uinput(m_fd_uinput);
#endif
}

bool CommonGpioReadThread::addDevice(const char * path, char pressedChar, char releasedChar, QObject *receiver, int keyId, bool uinputForward)
{
	DeviceData item;

	item.fd = -1;
	item.device = path;
	item.pressedChar = pressedChar;
	item.releasedChar = releasedChar;
	item.receiver = receiver;
	item.keyId = keyId;
	item.uinputForward = false;

#ifdef __linux__
	if ( uinputForward && m_fd_uinput >= 0 )
	{
		item.keyId = convQtKey2uinput(keyId);
		item.uinputForward = true;
		item.receiver = 0;
	}
#endif

#ifdef __linux__
	item.fd = ::open(item.device, O_RDONLY);
#endif

	if ( item.fd < 0 )
	{
		qDebug("[CommonGpioReadThread::addDevice] device open error: %s", item.device);
		return false;
	}

	stop();

	qDebug("[CommonGpioReadThread::addDevice] key 0x%x, device %s. %s" , item.keyId, item.device, uinputForward ? "[uinput]" : "[polling]");

	QMutexLocker locker(&m_mutex);
	m_data.append(item);
	m_stopFlag = false;
	start();
	return true;
}

void CommonGpioReadThread::closeDeviceFiles()
{
#ifdef __linux__
	for (int i = 0 ; i < m_data.size(); ++i)
	{
		if ( m_data[i].fd >= 0 )
			::close(m_data[i].fd);
	}
#endif
}

int CommonGpioReadThread::readDevice(int id)
{
	if ( id < 0 || id >= m_data.size() )
		return DEVICE_READ_ERROR;

#ifdef __linux__
	if ( m_data[id].fd < 0 )
	{
		qDebug("[CommonGpioReadThread::readDevice] device not opened : %s", m_data[id].device);
		return DEVICE_READ_ERROR;
	}

	char buf[4] = { 0, };

	::lseek(m_data[id].fd, 0, SEEK_SET);
	if ( ::read(m_data[id].fd, buf, sizeof(buf)) > 0 )
	{
		if ( buf[0] == m_data[id].pressedChar )
			return BUTTON_PRESSED;
		else if ( buf[0] == m_data[id].releasedChar )
			return BUTTON_RELEASED;
	}
	qDebug("[CommonGpioReadThread::readDevice] device read error : %s, [%s]", m_data[id].device, buf);
#endif
	return DEVICE_READ_ERROR;
}

void CommonGpioReadThread::run()
{
#ifdef __linux__
	if (m_data.size() == 0 )
	{
		qDebug("[CommonGpioReadThread::run] no device\n");
		return;
	}
	QList<int> prevStatus;

	for ( int i = 0 ; i < m_data.size(); ++i )
		prevStatus.append(readDevice(i));

	int stat;
	while ( m_stopFlag == false )
	{
		for (int i = 0 ; i < m_data.size() ; ++i )
		{
			stat = readDevice(i);
			if ( stat != DEVICE_READ_ERROR && stat != prevStatus[i])
			{
				qDebug("[CommonGpioReadThread::run] #%d -> %d", i, stat);

#ifdef __linux__
				// if uinput is available, generate uinput keystroke (kernel 3.4.104q)
				if ( m_data[i].uinputForward )
				{
#if 0
					// emulate key press/release event separately (it generates too many key event)
					send_keyboard_event(m_fd_uinput, m_data[i].keyId, stat == BUTTON_PRESSED ? 1 : 0 );
#endif
					// emulate keyboard press event only once when gpio button pressed
					if ( stat == BUTTON_PRESSED )
						press_keyboard_once(m_fd_uinput, m_data[i].keyId );
				}
#endif
				// old [start] key (kernel 3.4.104p)
				if ( m_data[i].receiver )
				{
					QKeyEvent * event = new QKeyEvent( stat == BUTTON_PRESSED ? QEvent::KeyPress : QEvent::KeyRelease, m_data[i].keyId, Qt::NoModifier);
					QCoreApplication::postEvent(m_data[i].receiver, event, Qt::HighEventPriority );
				}
			}
			prevStatus[i] = stat;
		}
		msleep(DEVICE_READ_INTERVAL_MSEC);

		int errCount = 0;

		for ( int i = 0 ; i < prevStatus.size(); ++i )
			if ( prevStatus[i] == DEVICE_READ_ERROR )
				++errCount;

		if ( errCount == prevStatus.size() )
			m_stopFlag = true;
	}
#endif
	qDebug("[CommonGpioReadThread::run] stopped");
}

bool CommonGpioReadThread::isPressed(const char *path)
{
	int idx = findDeviceIdx(path);
	if ( idx < 0 )
	{
		qDebug("[CommonGpioReadThread::isPressed] invalid device path [%s]", path);
		return false;
	}

	if ( readDevice(idx) == BUTTON_PRESSED )
		return true;

	return false;
}

void CommonGpioReadThread::setReceiver(const char *path, QObject *receiver, int keyId, bool uinputForward)
{
	int idx = findDeviceIdx(path);
	if ( idx < 0 )
	{
		qDebug("[CommonGpioReadThread::setReceiver] invalid device path [%s]", path);
		return;
	}

	m_data[idx].receiver = receiver;
	if ( keyId > -1)
		m_data[idx].keyId = keyId;

	m_data[idx].uinputForward = false;

#ifdef __linux__
	if ( uinputForward && m_fd_uinput >= 0 )
	{
		m_data[idx].keyId = convQtKey2uinput(keyId);
		m_data[idx].uinputForward = true;
		m_data[idx].receiver = 0;
	}
#endif
}

int CommonGpioReadThread::findDeviceIdx(const char *path)
{
	for ( int i = 0 ; i < m_data.size(); ++i )
		if ( ::strcmp(m_data[i].device, path) == 0 )
			return i;

	return -1;
}
