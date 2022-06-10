#ifndef COMMONGPIOREADTHREAD_H
#define COMMONGPIOREADTHREAD_H

#include <QThread>
#include <QMutex>

typedef struct device_data_rec
{
	const char * device;
	char pressedChar;
	char releasedChar;

	QObject *receiver;
	int keyId;
	bool uinputForward;

	int fd;
} DeviceData;


class CommonGpioReadThread : public QThread
{
	Q_OBJECT

private:
	explicit CommonGpioReadThread(QObject *parent = 0);
	~CommonGpioReadThread();

public:
	static CommonGpioReadThread& instance()
	{
		static CommonGpioReadThread gpio_read_thread;
		return gpio_read_thread;
	}

	void stop()
	{
		QMutexLocker locker(&m_mutex);

		m_stopFlag = true;
		wait();
	}

	bool addDevice(const char * path, char pressedChar = '0', char releasedChar = '1', QObject * receiver = 0, int keyId = -1, bool uinputForward = false);
	void setReceiver(const char *path, QObject *receiver, int keyId = -1, bool uinputForward = false);
	bool isPressed(const char *path); // true - pressed

protected:
	void closeDeviceFiles();
	int readDevice(int id);
	int findDeviceIdx(const char *path);

	void run();

private:

	QList <DeviceData> m_data;

	bool m_stopFlag;

	enum Constants
	{
		DEVICE_READ_INTERVAL_MSEC = 100, // TO-DO : adjust interval
		BUTTON_RELEASED = 0,
		BUTTON_PRESSED = 1,
		DEVICE_READ_ERROR = -1
	};

	QMutex m_mutex;

	int m_fd_uinput;
};

#endif // COMMONGPIOREADTHREAD_H
