#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
	Q_OBJECT
public:
	Worker(QObject *parent = 0);
	~Worker();

	unsigned int getCounter() const { return counter; }

protected:
	void timerEvent(QTimerEvent *event);

private:
	unsigned int counter;

	int mTimerId;
};

#endif // WORKER_H
