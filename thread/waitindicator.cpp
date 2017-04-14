#include "waitindicator.h"

#include <QPainter>

WaitIndicator::WaitIndicator(QWidget *parent) :
	QWidget(parent),
	m_angle(0),
	m_timerId(-1),
	m_delay(50),
	m_displayedWhenStopped(true),
	m_color(Qt::black)
{
	//setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFocusPolicy(Qt::NoFocus);
}

bool WaitIndicator::isAnimated () const
{
	return (m_timerId != -1);
}

void WaitIndicator::setDisplayedWhenStopped(bool state)
{
	m_displayedWhenStopped = state;

	update();
}

bool WaitIndicator::isDisplayedWhenStopped() const
{
	return m_displayedWhenStopped;
}

void WaitIndicator::startAnimation()
{
	m_angle = 0;

	if (m_timerId == -1)
		m_timerId = startTimer(m_delay);
}

void WaitIndicator::stopAnimation()
{
	if (m_timerId != -1)
		killTimer(m_timerId);

	m_timerId = -1;

	update();
}

void WaitIndicator::setAnimationDelay(int delay)
{
	if (m_timerId != -1)
		killTimer(m_timerId);

	m_delay = delay;

	if (m_timerId != -1)
		m_timerId = startTimer(m_delay);
}

void WaitIndicator::setColor(const QColor & color)
{
	m_color = color;

	update();
}

QSize WaitIndicator::sizeHint() const
{
	return QSize(20,20);
}

int WaitIndicator::heightForWidth(int w) const
{
	return w;
}

void WaitIndicator::timerEvent(QTimerEvent * /*event*/)
{
	m_angle = (m_angle+30)%360;

	update();
}

void WaitIndicator::paintEvent(QPaintEvent * /*event*/)
{
	if (!m_displayedWhenStopped && !isAnimated())
		return;

	int width = qMin(this->width(), this->height());

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	int outerRadius = (width-1) * 0.5;
	int innerRadius = (width-1) * 0.5 * 0.40;

	int capsuleHeight = outerRadius - innerRadius;
	int capsuleWidth  = capsuleHeight * 0.3;
	int capsuleRadius = (width > 32) ? capsuleWidth / 4.5 : capsuleWidth / 2.0;


	for (int i=0; i<12; i++)
	{
		QColor color = m_color;
		if (i < 5)
			color.setAlphaF(1.0 - (i/7.0));
		else
			color.setAlphaF(0.2);
		p.setPen(Qt::NoPen);
		p.setBrush(color);
		p.save();
		p.translate(rect().center());
		p.rotate(m_angle - (i*30.0));
		p.drawRoundedRect(-capsuleWidth*0.5, -(innerRadius+capsuleHeight), capsuleWidth, capsuleHeight, capsuleRadius, capsuleRadius);
		p.restore();
	}
}
