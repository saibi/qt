#ifndef SDOBSERVER_H
#define SDOBSERVER_H

#include <QThread>

#include <QMutex>
#include <QWaitCondition>

class SdObserver : public QThread
{
    Q_OBJECT

public:
	explicit SdObserver(QObject *parent = 0);

	bool startObserver(const QString & id = 0);
	void stopObserver();

protected:
    void run();

    //
    bool sdExists();

signals:
	void signalSdMounted();
    void signalSdUnmounted();

public slots:

private:
	volatile bool _stopFlag;

    QMutex _mutex;

    bool _sdFlag;
    bool _mountFlag;

	QString m_id;
};

#endif // SDOBSERVER_H
