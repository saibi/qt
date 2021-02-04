#ifndef MYTIMER_H
#define MYTIMER_H

#include <QObject>
#include <QTimer>

class MyTimer : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
	Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)


public:
	explicit MyTimer(QObject *parent = nullptr);
	void setInterval(int msec);
	int interval();
	bool isActive();

public slots:
	void start();
	void stop();

signals:
	void timeout();
	void intervalChanged();
	void activeChanged();

private:
	QTimer * m_timer;
};

#endif // MYTIMER_H
