#ifndef PROGRESSTHREAD_H
#define PROGRESSTHREAD_H

#include <QThread>
#include "dlg_progress.h"

class ProgressThread : public QThread
{
	Q_OBJECT
public:
	explicit ProgressThread(QObject *parent = 0);
	
	void setMaxTime(int seconds);
	void completeProgressBar();
	void setPercent(float percent);
signals:
	
public slots:
	

protected:
	void run();

private:
	Dlg_Progress m_progressBar;
	int m_maxTime;
	float m_percent;

	int m_statusFlag;
	enum {
		PT_FLAG_START = 0x01,
		PT_FLAG_PERCENT = 0x02,
		PT_FLAG_COMPLETE = 0x04
	};
};

#endif // PROGRESSTHREAD_H
