#ifndef SDOBSERVER3_H
#define SDOBSERVER3_H

#include <QThread>

#include <QMutex>
#include <QWaitCondition>

class SdObserver3 : public QThread
{
    Q_OBJECT

private:
	explicit SdObserver3(QObject *parent = 0);

	Q_DISABLE_COPY(SdObserver3)

public:
	static SdObserver3& instance()
    {
		static SdObserver3 singletonInstance;
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

#endif // SDOBSERVER3_H
