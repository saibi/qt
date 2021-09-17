#ifndef INPUTTHREAD_H
#define INPUTTHREAD_H

#include <QThread>

class InputThread : public QThread
{
	Q_OBJECT

public:
	InputThread(QObject *parent = 0);

	void run();

signals:
	void signal_line(QString line);

private:

	bool m_stopFlag;
};

#endif // INPUTTHREAD_H
