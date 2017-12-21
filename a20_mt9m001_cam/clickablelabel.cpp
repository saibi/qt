#include "clickablelabel.h"

#include <QMouseEvent>
#include <QTimerEvent>

#define LONG_PRESS_MSEC 1200

ClickableLabel::ClickableLabel(QWidget *parent) :
	QLabel(parent)
{
	m_longPressMSec = LONG_PRESS_MSEC;
	m_lastClickTime = QTime::currentTime();

	m_longPressEnabled = false;
	m_longPressTimerId = 0;
	m_longPressed = false;
	m_pressReleaseEnabled = false;
}

void ClickableLabel::mousePressEvent(QMouseEvent *e)
{
	if (m_longPressTimerId)
	{
		killTimer(m_longPressTimerId);
		m_longPressTimerId = 0;
	}

	if (m_longPressEnabled)
	{
		m_longPressTimerId = startTimer(m_longPressMSec);
		m_longPressed = false;
	}
	if ( m_pressReleaseEnabled )
		emit pressed(e->pos());
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *e)
{
	if (m_longPressTimerId)
	{
		killTimer(m_longPressTimerId);
		m_longPressTimerId = 0;
	}

	if (m_longPressEnabled && m_longPressed)
	{
		// none - pass
		m_longPressed = false;
	}
	else if (this->isEnabled())
	{
		int diffMsecs = m_lastClickTime.msecsTo(QTime::currentTime());

		if (this->rect().contains(e->pos())
				&& (diffMsecs > 100 || diffMsecs < 0))
		{
			emit clicked(e->pos());
			m_lastClickTime = QTime::currentTime();
		}
	}

	if ( m_pressReleaseEnabled )
		emit released(e->pos());
}

void ClickableLabel::timerEvent(QTimerEvent *e)
{
	if (m_longPressTimerId &&
			e->timerId() == m_longPressTimerId)
	{
		killTimer(m_longPressTimerId);
		m_longPressTimerId = 0;
	}

	m_longPressed = true;

	emit longPressed();
}
