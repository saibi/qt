#include "notification.h"

#include <QMutexLocker>
#include <QApplication>

Notification::Notification(QObject *parent) :
	QObject(parent),
	m_mutex(QMutex::Recursive)
{
	m_notifyWidget = new NotificationWidget();

	// timer has error. If there is no time out after starting SW, time out is not working properly.
	// So, we must make time out forcely.
	// ex) timer start -> before expired, kill timer -> another timer start -> there is no time out.
	m_notifyTimerId = startTimer(500);
//	m_notifyTimerId = 0;

	// For upper problem, you must make this object at starting SW to make timer expired.
	// at 120510 by bmkim
}

Notification::~Notification()
{
//	Q_CHECK_PTR(m_notifyWidget);
//	delete m_notifyWidget;
}

void Notification::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);
//	qDebug("[Notification::timerEvent] timer id: %d", event->timerId());
	hide();
}

void Notification::notify(const QString &message, int notifySecond, bool refreshNow)
{
	QMutexLocker locker(&m_mutex);

//	qDebug("[Notification::notify] timer id: %d", m_notifyTimerId);
//	qDebug("[Notification::notify] %s", qPrintable(message));
	if (m_notifyTimerId)
	{
		killTimer(m_notifyTimerId);
//		qDebug("[Notification::notify] timer killed - id: %d", m_notifyTimerId);
		m_notifyTimerId = 0;
	}

	m_notifyWidget->hide();
	m_notifyWidget->setText(message);
	m_notifyWidget->show();
	m_notifyWidget->update();

	if (notifySecond > 0)
	{
		m_notifyTimerId = startTimer(notifySecond * 1000);
//		qDebug("[Notification::notify] start timer - id: %d", m_notifyTimerId);
	}

	if ( refreshNow )
		QApplication::processEvents();
}

void Notification::hide()
{
	QMutexLocker locker(&m_mutex);

//	qDebug("[Notification::hide] timer id: %d", m_notifyTimerId);
	if (m_notifyTimerId)
	{
		killTimer(m_notifyTimerId);
//		qDebug("[Notification::hide] timer killed - id: %d", m_notifyTimerId);
		m_notifyTimerId = 0;
	}

	m_notifyWidget->hide();

	//QApplication::processEvents();
}

void Notification::hide(const QString &message)
{
	QMutexLocker locker(&m_mutex);

	if(m_notifyWidget->text() != message)
		return;

//	qDebug("[Notification::hide] timer id: %d", m_notifyTimerId);
	if (m_notifyTimerId)
	{
		killTimer(m_notifyTimerId);
//		qDebug("[Notification::hide] timer killed - id: %d", m_notifyTimerId);
		m_notifyTimerId = 0;
	}

	m_notifyWidget->hide();

	//QApplication::processEvents();
}

