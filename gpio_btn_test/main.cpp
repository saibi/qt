#include <QApplication>
#include "commongpioreadthread.h"
#include "receiver.h"

#define USE_POLLING

#define POWER_BTN_DEVICE_PATH "/sys/class/gpio/gpio3_ph13/value"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Receiver r;

#ifdef USE_POLLING
	CommonGpioReadThread::instance().addDevice(POWER_BTN_DEVICE_PATH, '0', '1', &r, Qt::Key_F1);
#endif

#ifdef USE_LINUX_UINPUT
	CommonGpioReadThread::instance().addDevice(POWER_BTN_DEVICE_PATH, '0', '1', 0, Qt::Key_F1, true);
	r.show();
#endif

	return a.exec();
}
