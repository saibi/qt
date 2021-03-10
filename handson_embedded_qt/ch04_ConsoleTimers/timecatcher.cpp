#include "timecatcher.h"

#include <QTextStream>

TimeCatcher::TimeCatcher(QObject *parent) : QObject(parent), m_count(0)
{
	connect(this, &TimeCatcher::newCount, this, &TimeCatcher::printCount, Qt::QueuedConnection);
	connect(&m_timer, &QTimer::timeout, this, &TimeCatcher::catchExpiredTimer);
	m_timer.setSingleShot(false);
	m_timer.start(5000);
}

void TimeCatcher::catchExpiredTimer()
{
	emit newCount(++m_count);
	QTextStream(stdout) << "Got Timeout!" << Qt::endl;
}

void TimeCatcher::printCount(int count)
{
	QTextStream(stdout) << "count = " << count << Qt::endl;
}
