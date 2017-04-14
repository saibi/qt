#ifndef SDOBSERVER_H
#define SDOBSERVER_H

#include <QThread>

#include <QMutex>
#include <QWaitCondition>

//#define _SD_MOUNT_SIMULATION_

#if defined(_SD_MOUNT_SIMULATION_) || !defined(__arm__)
#include <QCoreApplication>
#endif

class SdObserver : public QThread
{
    Q_OBJECT

private:
    explicit SdObserver(QObject *parent = 0);

	Q_DISABLE_COPY(SdObserver)

public:
    static SdObserver& instance()
    {
        static SdObserver singletonInstance;
        return singletonInstance;
    }

#if defined(__arm__) && ! defined(_SD_MOUNT_SIMULATION_)
    bool startObserver(const QString &sdMountPath = QString("/mnt/sdcard"));
#else
	bool startObserver(const QString &sdMountPath = QString(QCoreApplication::applicationDirPath() + "/mnt/sdcard"));
#endif

    void stopObserver();

    bool isMounted();

	QString getMountPath();

protected:
    void run();

    //
    bool sdExists();

    bool autoMount();

    bool autoUnmount();

	bool usbReaderConnected();
	bool usbSdExists();
	bool checkFdiskResultContains(QString contains);
	QString getPartitionName(QString start_str, QString end_str);
	bool usbAutoMount();
	bool usbAutoUnmount();

signals:
    void signalSdMounted(QString sdMountPath);

    void signalSdUnmounted();
	void signalSdMountError();

public slots:

private:
    bool _stopFlag;

    QMutex _mutex;

    QString _sdMountPath;

    bool _sdFlag;

    bool _mountFlag;

    QString _mmcblkFileName;

    int _errorCount;

	bool _usb_sd_flag;
	QString _usbsd_dev_name;
	bool _usb_mount_flag;
};

#endif // SDOBSERVER_H
