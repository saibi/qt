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
	void newCount (int count);

public slots:
	void catchExpiredTimer(); ///< called when m_timer timesout
	void printCount(int count);

private:
	QTimer m_timer;
	int m_count;
};

#endif // TIMECATCHER_H
