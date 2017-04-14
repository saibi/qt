#include "worker.h"

Worker::Worker()
{
	m_count = 0;
	qDebug("[Worker] created");

}

Worker::Worker(QString name)
{
	m_count = 0;
	m_name = name;

	qDebug("[Worker] %s created", m_name.toLocal8Bit().constData());
}

Worker::~Worker()
{
}

void Worker::process()
{
	qDebug("[Worker::process] [%s] start", m_name.toLocal8Bit().constData());

	while( m_count < 1000 )
	{
		m_count ++;
		emit progress(m_name, m_count);
		qDebug("[Worker::process()] [%s] %d", m_name.toLocal8Bit().constData(), m_count);
		sleep(1);
	}
	m_count = 0;
	emit finished();
}
