#include "timecatcher.h"

#include <QTextStream>

TimeCatcher::TimeCatcher(QObject *parent) : QObject(parent)
{
	connect(&m_timer, &QTimer::timeout, this, &TimeCatcher::catchExpiredTimer);
	m_timer.setSingleShot(false);
	m_timer.start(5000);
}

void TimeCatcher::catchExpiredTimer()
{
	QTextStream(stdout) << "Got Timeout!" << Qt::endl;
}
