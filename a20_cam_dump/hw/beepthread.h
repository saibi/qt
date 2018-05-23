#ifndef BEEPTHREAD_H
#define BEEPTHREAD_H

#include <QThread>

#include <QMutex>
#include <QWaitCondition>
#include <QQueue>

class BeepThread : public QThread
{
	Q_OBJECT

public:
	explicit BeepThread(QObject *parent = 0);

	void setData(int fdBeep)
	{
		_fdBeep = fdBeep;
	}

	void stop();

	void beep(unsigned int beepMsecs, unsigned int muteMsecs);

	void beep(unsigned int beep1Msecs, unsigned int mute1Msecs,
			  unsigned int beep2Msecs, unsigned int mute2Msecs);

	void beep(unsigned int beep1Msecs, unsigned int mute1Msecs,
			  unsigned int beep2Msecs, unsigned int mute2Msecs,
			  unsigned int beep3Msecs, unsigned int mute3Msecs);

protected:
	void beep_i(unsigned int beepMsecs, unsigned int muteMsecs);

	void run();

private:
	int _fdBeep;

	bool _stopFlag;

	QMutex _mutex;

	QWaitCondition _waitCondition;

	QQueue<unsigned int> _beepMuteQueue;
};

#endif // BEEPTHREAD_H
