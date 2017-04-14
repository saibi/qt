#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QObject>

#include <QMutex>

#include "notificationwidget.h"

class Notification : public QObject
{
	Q_OBJECT

private:
	explicit Notification(QObject *parent = 0);
	~Notification();

	Q_DISABLE_COPY(Notification)

public:
	static Notification& instance()
	{
		static Notification notification;
		return notification;
	}

	//
	void notify(const QString& message, int notifySecond = 3, bool refreshNow = true);

	void hide();
	void hide(const QString &message);

protected:
	virtual void timerEvent(QTimerEvent *);

private:
	QMutex m_mutex;

	NotificationWidget *m_notifyWidget;

	int m_notifyTimerId;

signals:

public slots:

};

#endif // NOTIFICATION_H
