#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{

  Q_OBJECT

public:
	MyThread();

public slots:
	void stop()
	{
		m_stopFlag = true;
		wait();
	}

private:
	void run();

signals:
	void updateThread(int count);

private:
	bool m_stopFlag;
};
#endif
