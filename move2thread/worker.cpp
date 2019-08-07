#include "worker.h"
#include <QDebug>

Worker::Worker(QObject *parent) :
	QObject(parent),
	counter(0),
	mTimerId(0)
{
	mTimerId = startTimer(1000);

	qDebug() << "Worker()";
}

void Worker::timerEvent(QTimerEvent * /* event */)
{
	++counter;
	qDebug() << "# " << counter;
}


Worker::~Worker()
{
	killTimer(mTimerId);
	qDebug() << "~Worker()";
}
