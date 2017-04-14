#ifndef SDOBSERVER2_H
#define SDOBSERVER2_H

#include <QThread>

#include <QMutex>
#include <QWaitCondition>

class SdObserver2 : public QThread
{
    Q_OBJECT

private:
	explicit SdObserver2(QObject *parent = 0);

	Q_DISABLE_COPY(SdObserver2)

public:
	static SdObserver2& instance()
    {
		static SdObserver2 singletonInstance;
        return singletonInstance;
    }

	bool startObserver();

    void stopObserver();

protected:
    void run();

    bool sdExists();

signals:
	void signalSdMounted();
    void signalSdUnmounted();

public slots:

private:
    bool _stopFlag;

    QMutex _mutex;

    bool _sdFlag;
    bool _mountFlag;

};

#endif // SDOBSERVER2_H
