#include "mytimer.h"
#include <QDebug>

MyTimer::MyTimer(QObject *parent) : QObject(parent), m_timer( new QTimer(this) )
{
	qDebug() << "MyTimer()";
	connect(m_timer, SIGNAL(timeout()), this, SIGNAL(timeout()));

}

void MyTimer::setInterval(int msec)
{
	if (m_timer->interval() == msec )
		return;

	m_timer->setInterval(msec);
	emit intervalChanged();
}


int MyTimer::interval()
{
	return m_timer->interval();
}


bool MyTimer::isActive()
{
	return m_timer->isActive();
}


void MyTimer::start()
{
	if (m_timer->isActive() )
			return ;

	m_timer->start();
	emit activeChanged();
}

void MyTimer::stop()
{
	if ( !m_timer->isActive() )
		return;

	m_timer->stop();
	emit activeChanged();
}
