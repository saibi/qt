#include "thread.h"
#include <iostream>

Thread::Thread()
{
	stopped = false;
}

void Thread::run()
{
	forever {
		QMutexLocker locker(&mutex);
		if (stopped) {
			stopped = false;
			break;
		}
		std::cerr << qPrintable(messageStr);
	}
	std::cerr << std::endl;
#if 0
	forever {
		mutex.lock();
		if (stopped) {
			stopped = false;
			mutex.unlock();
			break;
		}
		mutex.unlock();
		std::cerr << qPrintable(messageStr);
	}
	std::cerr << std::endl;
#endif 
}


void Thread::stop()
{
	QMutexLocker locker(&mutex);
	stopped = true;
#if 0
	mutex.lock();
	stopped = true;
	mutex.unlock();
#endif 
}

void Thread::setMessage(const QString &message)
{
	messageStr = message;
}

