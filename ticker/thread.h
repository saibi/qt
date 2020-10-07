#ifndef THREAD_H_
#define THREAD_H_

#include <QThread>
#include <QMutex>


class Thread : public QThread
{
	Q_OBJECT
	
public:
	Thread();

	void setMessage(const QString &message);
	void stop();

protected:
	void run();

private:
	QString messageStr;
	volatile bool stopped;
	QMutex mutex;
};

#endif
