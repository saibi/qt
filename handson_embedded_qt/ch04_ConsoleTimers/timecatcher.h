#ifndef TIMECATCHER_H
#define TIMECATCHER_H

#include <QObject>
#include <QTimer>

class TimeCatcher : public QObject
{
	Q_OBJECT
public:
	explicit TimeCatcher(QObject *parent = nullptr);

signals:

public slots:
	void catchExpiredTimer(); ///< called when m_timer timesout

private:
	QTimer m_timer;
};

#endif // TIMECATCHER_H
