#include "progressthread.h"

ProgressThread::ProgressThread(QObject *parent) :
	QThread(parent)
{
	m_maxTime = 0;
	m_percent = 0.0;
}

void ProgressThread::setMaxTime(int seconds)
{
	m_maxTime = seconds;
}

void ProgressThread::setPercent(float percent)
{
	if ( m_statusFlag & PT_FLAG_START )
	{
		if ( percent > 100.0 )
		{
			m_percent = 100.0;
		}
		else if ( percent < 0.0 )
		{
			m_percent = 0.0;
		}
		else
		{
			m_percent = percent;
		}

		m_statusFlag |= PT_FLAG_PERCENT;
	}
	else
	{
		qDebug("thread not started");
	}
}

void ProgressThread::completeProgressBar()
{
	if ( m_statusFlag & PT_FLAG_START )
	{
		m_statusFlag |= PT_FLAG_COMPLETE;
	}
	else
	{
		qDebug("thread not started");
	}
}

void ProgressThread::run()
{
	if ( m_maxTime <= 0 )
	{
		qDebug("max time not set");
		return;
	}

	m_statusFlag = PT_FLAG_START;

	m_progressBar.setTime( m_maxTime );
	m_progressBar.show();

	while ( m_progressBar.isStarted() )
	{
		if ( m_statusFlag & PT_FLAG_PERCENT )
		{
			m_progressBar.setPercent(m_percent);
			m_statusFlag &= (~PT_FLAG_PERCENT);
		}
		else if ( m_statusFlag & PT_FLAG_COMPLETE )
		{
			m_progressBar.terminate();
			m_statusFlag &= (~PT_FLAG_COMPLETE);
		}
		this->usleep(1000);
	}
}


