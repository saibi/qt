#include "sdobserver.h"

#include <QMutexLocker>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>

#define SD_CHECK_MSEC 1000

SdObserver::SdObserver(QObject *parent) :
    QThread(parent),
    _mutex(QMutex::Recursive)
{
    _stopFlag = false;
}

bool SdObserver::startObserver(const QString &sdMountPath)
{
    qDebug("[SdObserver::startObserver] Start.");

    if (this->isRunning())
    {
        stopObserver();
    }

    //
    QFileInfo mountPathFileInfo(sdMountPath);

    if (mountPathFileInfo.exists())
    {
        // exist
        if (!mountPathFileInfo.isDir())
        {
			qDebug("[SdObserver::startObserver] Path: [%s] exists, but is not directory.",
                      sdMountPath.toLocal8Bit().constData());
            return false;
        }
    }
    else
    {
        // not exist - mkpath
        QDir mountDir;
        if (!mountDir.mkpath(sdMountPath))
        {
			qDebug("[SdObserver::startObserver] Path: [%s] not exists, and can not make path.",
                      sdMountPath.toLocal8Bit().constData());
            return false;
        }
    }

    _sdMountPath = sdMountPath;

	//
	QMutexLocker locker(&_mutex);

	_stopFlag = false;

    start();

    qDebug("[SdObserver::startObserver] Done.");

    return true;
}

void SdObserver::stopObserver()
{
    QMutexLocker locker(&_mutex);

    _stopFlag = true;

    locker.unlock();

    wait();
}

bool SdObserver::isMounted()
{
    QMutexLocker locker(&_mutex);

    if (!this->isRunning())
        return false;

#if defined(_SD_MOUNT_SIMULATION_)
	return _mountFlag;
#elif defined(__arm__)
	return _mountFlag | _usb_mount_flag;
#else
	return true;
#endif
}

QString SdObserver::getMountPath()
{
	QMutexLocker locker(&_mutex);

	return _sdMountPath;
}

void SdObserver::run()
{
    qDebug("[SdObserver::run] Start.");

    // check already mounted sd card & umount
    // TODO

    //
    bool sdFlag;

    // reset flag
    _sdFlag = false;
    _mountFlag = false;
    _errorCount = 0;

	bool usb_sd_flag;
	_usb_sd_flag = false;
	_usb_mount_flag = false;

    while (1)
    {
        if (_stopFlag)
            break;

        sdFlag = sdExists();

        if (sdFlag != _sdFlag)
        {
            if (sdFlag)
            {
                qDebug("[SdObserver::run] SD inserted. - [%s]", _mmcblkFileName.toLocal8Bit().constData());

				// 기존 SD 카드 리더를 통해 mount 수행 시, usb 측을 반드시 umount
				if(_usb_mount_flag)
				{
					usbAutoUnmount();

					qDebug("[SdObserver::run] usb sd unmounted.");
					emit signalSdUnmounted();
					_usb_mount_flag = false;
					_usb_sd_flag = false;

					this->msleep(SD_CHECK_MSEC);
					continue;
				}

                // inserted
                if (!autoMount())
                {
                    _errorCount++;
					qDebug("[SdObserver::run] Failed to autoMount - Error Count: [%d]", _errorCount);

					if ( _errorCount == 10 || _errorCount == 20 || _errorCount == 30 )
						emit signalSdMountError();

                    if (_errorCount > 3)
                        this->msleep(SD_CHECK_MSEC);

                    continue;
                }

                _errorCount = 0;

                emit signalSdMounted(_sdMountPath);

                _mountFlag = true;
            }
            else
            {
                qDebug("[SdObserver::run] SD removed.");

                // removed
                if (!autoUnmount())
                {
                    _errorCount++;
					qDebug("[SdObserver::run] Failed to autoUnmount - Error Count: [%d]", _errorCount);

                    if (_errorCount > 3)
                        this->msleep(SD_CHECK_MSEC);

                    continue;
                }

                _errorCount = 0;

                emit signalSdUnmounted();

                _mountFlag = false;
            }

            _sdFlag = sdFlag;
        }
		else if(sdFlag == false) // 기존 SD 카드 리더기 상태 변화가 없음 + 기존 SD 카드 리더기에 SD 카드가 없음
		{
			if(usbReaderConnected())
			{
				usb_sd_flag = usbSdExists();
				if(usb_sd_flag != _usb_sd_flag)
				{
					if(usb_sd_flag)
					{
						qDebug("[SdObserver::run] USB SD inserted. - [%s]", qPrintable(_usbsd_dev_name));

						// inserted
						if (!usbAutoMount())
						{
							qDebug("[SdObserver::run] Failed to usbAutoMount");
							_errorCount++;

							if ( _errorCount == 10 || _errorCount == 20 || _errorCount == 30 )
								emit signalSdMountError();

							this->msleep(SD_CHECK_MSEC);
							continue;
						}

						_errorCount = 0;

						emit signalSdMounted(_sdMountPath);
						_usb_mount_flag = true;
					}
					else
					{
						qDebug("[SdObserver::run] USB SD removed.");

						// removed
						if (!usbAutoUnmount())
						{
							qDebug("[SdObserver::run] Failed to autoUnmount");
							this->msleep(SD_CHECK_MSEC);
							continue;
						}

						emit signalSdUnmounted();
						_usb_mount_flag = false;
						_errorCount = 0;
					}

					_usb_sd_flag = usb_sd_flag;
				}
			}
		}

        this->msleep(SD_CHECK_MSEC);
    }

    qDebug("[SdObserver::run] Stop.");
}

bool SdObserver::sdExists()
{
    //qDebug("[SdObserver::sdExists] Start.");

#if defined(_SD_MOUNT_SIMULATION_)
	if(QFile::exists(QCoreApplication::applicationDirPath() + "/unmounted") == false)
	{
		return true;
	}
#elif defined(__arm__)
    QFile partFile("/proc/partitions");

    if (partFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
    {
		qDebug("[SdObserver::sdExists] Failed to open '/proc/partitions' file.");
        return false;
    }

    QString partFileData = partFile.readAll().simplified();

    partFile.close();

    QStringList partFileDataSplits = partFileData.split(" ", QString::SkipEmptyParts);

    QStringList mmcblkList;

    for (int i = 0; i < partFileDataSplits.size(); i++)
    {
        const QString &dataSplit = partFileDataSplits.at(i);

        if (dataSplit.startsWith("mmcblk"))
        {
            mmcblkList.append(dataSplit);
        }
    }

    if (mmcblkList.size() > 0)
    {
        //qDebug("[SdObserver::sdExists] Done. - SD exists");

        // mmcblk0
        _mmcblkFileName = mmcblkList.at(0);

        for (int i = 0; i < mmcblkList.size(); i++)
        {
            if (mmcblkList.at(i).endsWith("p1"))
            {
                // mmcblk0p1
                _mmcblkFileName = mmcblkList.at(i);
                break;
            }
        }

        return true;
    }
#endif

    //qDebug("[SdObserver::sdExists] Done. - No SD");

    return false;
}

bool SdObserver::autoMount()
{
    qDebug("[SdObserver::autoMount] Start.");

    if (!autoUnmount())
    {
		qDebug("[SdObserver::autoMount] Failed to autoUnmount");
        return false;
    }

#if defined(_SD_MOUNT_SIMULATION_)
	return true;
#elif defined(__arm__)
    int ret;
    QString cmd;
    QString mountFileData;

    cmd.sprintf("mount -t vfat /dev/%s %s",
                _mmcblkFileName.toLocal8Bit().constData(),
                _sdMountPath.toLocal8Bit().constData());

    qDebug("[SdObserver::autoMount] Run - [%s]", cmd.toLocal8Bit().constData());

    ret = QProcess::execute(cmd);

    if (ret == -1 || ret == -2)
    {
		qDebug("[SdObserver::autoMount] Failed to mount - Error: [%d]", ret);
        return false;
    }
    else if (ret != 0)
    {
        qDebug("[SdObserver::autoMount] Failed to mount - Error: [%d]", ret);
    }

    // check - read /proc/mounts
    {
        QFile mountFile("/proc/mounts");

        if (mountFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
        {
			qDebug("[SdObserver::autoMount] Failed to open '/proc/mounts' file.");
            return false;
        }

        mountFileData = mountFile.readAll();

        mountFile.close();

        if (mountFileData.contains(_sdMountPath))
        {
            qDebug("[SdObserver::autoMount] Done.");

            return true;
        }
    }
#endif

	qDebug("[SdObserver::autoMount] Failed to mount - Check Failed");

    return false;
}

bool SdObserver::autoUnmount()
{
    qDebug("[SdObserver::autoUnmount] Start.");

#if defined(__arm__) && !defined(_SD_MOUNT_SIMULATION_)
    int i, ret;
    QString cmd;
    QString mountFileData;
    QStringList mountFileDataSplits;
    QStringList mountPosList;

    // read /proc/mounts
    {
        QFile mountFile("/proc/mounts");

        if (mountFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
        {
			qDebug("[SdObserver::autoUnmount] Failed to open '/proc/mounts' file.");
            return false;
        }

        mountFileData = mountFile.readAll();

        mountFile.close();
    }

    mountFileDataSplits = mountFileData.split("\n", QString::SkipEmptyParts);

    for (i = 0; i < mountFileDataSplits.size(); i++)
    {
        const QString &dataSplit = mountFileDataSplits.at(i).trimmed();

        if (dataSplit.startsWith("/dev/mmcblk"))
        {
            mountPosList.append(dataSplit.section(" ", 1, 1));
        }
    }

    if (mountPosList.size() == 0)
    {
        qDebug("[SdObserver::autoUnmount] Done. - No Mount");
        return true;
    }

    for (i = 0; i < mountPosList.size(); i++)
    {
        cmd.sprintf("umount %s", mountPosList.at(i).toLocal8Bit().constData());

        qDebug("[SdObserver::autoUnmount] Run - [%s]", cmd.toLocal8Bit().constData());

        ret = QProcess::execute(cmd);

        if (ret == -1 || ret == -2)
        {
			qDebug("[SdObserver::autoUnmount] Failed to umount - Error: [%d]", ret);
            return false;
        }
    }

    // check - read /proc/mounts
    {
        QFile mountFile("/proc/mounts");

        if (mountFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
        {
			qDebug("[SdObserver::autoUnmount] Failed to open '/proc/mounts' file. 2");
            return false;
        }

        mountFileData = mountFile.readAll();

        mountFile.close();

        if (mountFileData.contains("/dev/mmcblk"))
        {
			qDebug("[SdObserver::autoUnmount] Failed to umount - Check Failed");
            return false;
        }
    }
#endif

    qDebug("[SdObserver::autoUnmount] Done. - OK");

    return true;
}

bool SdObserver::usbReaderConnected()
{
#if defined(__arm__) && !defined(_SD_MOUNT_SIMULATION_)
	QDir dev_dir = QDir("/dev");

//	qDebug("[SdObserver::usbReaderConnected] path: %s", qPrintable(dev_dir.absolutePath()));

	QStringList scsi_disk_list = dev_dir.entryList(QStringList() << "sd*", QDir::System);

//	qDebug("[SdObserver::usbReaderConnected] scsi_disk_list (size:%d)", scsi_disk_list.size());
//	for(int i = 0 ; i < scsi_disk_list.count() ; i++)
//	{
//		qDebug("\t> %s", qPrintable(scsi_disk_list.at(i)));
//	}

	if(scsi_disk_list.count())
	{
		_usbsd_dev_name = scsi_disk_list.at(0); // sd*
		return true;
	}
#endif
	return false;
}

bool SdObserver::usbSdExists()
{
#if defined(__arm__) && !defined(_SD_MOUNT_SIMULATION_)
	bool found_sd_in_fdisk = false;
	bool found_sd_in_partitions = false;

	found_sd_in_fdisk = checkFdiskResultContains("/dev/sd");

	QString usbsd_dev_name = getPartitionName("sd", "1"); // sd*1
	if(usbsd_dev_name.endsWith("1"))
		found_sd_in_partitions = true;

	if(usbsd_dev_name.isEmpty() == false)
		_usbsd_dev_name = usbsd_dev_name;

	if(found_sd_in_fdisk && found_sd_in_partitions)
		return true;
	else if(!found_sd_in_fdisk && found_sd_in_partitions)
	{
		// mount 상태에서 sd 카드가 빠진 상태
		// umount 수행 해주어야 함
		usbAutoUnmount();
	}

#endif
	return false;
}

bool SdObserver::checkFdiskResultContains(QString contains)
{
#if defined(__arm__) && !defined(_SD_MOUNT_SIMULATION_)
	QProcess process;
	QString fdisk_cmd, fdisk_output;

	// 장치명을 명시하여 주면, kernel의 media revalidate도 강제 수행된다.
	fdisk_cmd = "fdisk -l /dev/" + _usbsd_dev_name;

//	qDebug("[SdObserver::checkFdiskResult] Run - [%s]", qPrintable(fdisk_cmd));

	process.start(fdisk_cmd);
	process.waitForFinished();
	fdisk_output = QString(process.readAllStandardOutput()).simplified();

//	qDebug("[SdObserver::checkFdiskResult] Result\n%s", qPrintable(fdisk_output));

	if(fdisk_output.contains(contains))
		return true;
#else
	Q_UNUSED(contains);
#endif

	return false;
}

QString SdObserver::getPartitionName(QString start_str, QString end_str)
{
	QString partition_name = "";
#if defined(__arm__) && !defined(_SD_MOUNT_SIMULATION_)
	QFile partition_file("/proc/partitions");

	if (partition_file.open(QIODevice::ReadOnly | QIODevice::Text) == false)
	{
		qDebug("[SdObserver::getPartitionName] Failed to open '/proc/partitions' file.");
		return partition_name;
	}

	QString partition_data = partition_file.readAll().simplified();

	partition_file.close();

	QStringList partition_list = partition_data.split(" ", QString::SkipEmptyParts);
	QStringList target_list;

	for (int i = 0; i < partition_list.size(); i++)
	{
		const QString &partition = partition_list.at(i);

		if (partition.startsWith(start_str))
		{
			target_list.append(partition);
		}
	}

	if (target_list.size() > 0)
	{
		partition_name = target_list.at(0);

		for (int i = 0; i < target_list.size(); i++)
		{
			if (target_list.at(i).endsWith(end_str))
			{
				partition_name = target_list.at(i);
				break;
			}
		}
	}
#else
	Q_UNUSED(start_str);
	Q_UNUSED(end_str);
#endif

	return partition_name;
}

bool SdObserver::usbAutoMount()
{
	qDebug("[SdObserver::usbAutoMount] Start.");

	if (!usbAutoUnmount())
	{
		qDebug("[SdObserver::usbAutoMount] Failed to usbAutoUnmount");
		return false;
	}

#if defined(__arm__) && !defined(_SD_MOUNT_SIMULATION_)
	int ret;
	QString cmd;
	QString mountFileData;

	cmd.sprintf("mount -t vfat /dev/%s %s",
				qPrintable(_usbsd_dev_name),
				qPrintable(_sdMountPath));

	qDebug("[SdObserver::usbAutoMount] Run - [%s]", cmd.toLocal8Bit().constData());

	ret = QProcess::execute(cmd);

	if (ret == -1 || ret == -2)
	{
		qDebug("[SdObserver::usbAutoMount] Failed to mount - Error: [%d]", ret);
		return false;
	}
	else if (ret != 0)
	{
		qDebug("[SdObserver::usbAutoMount] Failed to mount - Error: [%d]", ret);
	}

	// check - read /proc/mounts
	{
		QFile mountFile("/proc/mounts");

		if (mountFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
		{
			qDebug("[SdObserver::usbAutoMount] Failed to open '/proc/mounts' file.");
			return false;
		}

		mountFileData = mountFile.readAll();

		mountFile.close();

		if (mountFileData.contains(_sdMountPath))
		{
			qDebug("[SdObserver::usbAutoMount] Done.");

			return true;
		}
	}
#endif

	qDebug("[SdObserver::usbAutoMount] Failed to mount - Check Failed");

	return false;
}

bool SdObserver::usbAutoUnmount()
{
	qDebug("[SdObserver::usbAutoUnmount] Start.");

#if defined(__arm__) && !defined(_SD_MOUNT_SIMULATION_)
	int i, ret;
	QString cmd;
	QString mountFileData;
	QStringList mountFileDataSplits;
	QStringList mountPosList;

	// read /proc/mounts
	{
		QFile mountFile("/proc/mounts");

		if (mountFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
		{
			qDebug("[SdObserver::usbAutoUnmount] Failed to open '/proc/mounts' file.");
			return false;
		}

		mountFileData = mountFile.readAll();

		mountFile.close();
	}

	mountFileDataSplits = mountFileData.split("\n", QString::SkipEmptyParts);

	for (i = 0; i < mountFileDataSplits.size(); i++)
	{
		const QString &dataSplit = mountFileDataSplits.at(i).trimmed();

		if (dataSplit.startsWith("/dev/sd"))
		{
			mountPosList.append(dataSplit.section(" ", 1, 1));
		}
	}

	if (mountPosList.size() == 0)
	{
		qDebug("[SdObserver::usbAutoUnmount] Done. - No Mount");
		return true;
	}

	for (i = 0; i < mountPosList.size(); i++)
	{
		cmd.sprintf("umount %s", mountPosList.at(i).toLocal8Bit().constData());

		qDebug("[SdObserver::usbAutoUnmount] Run - [%s]", cmd.toLocal8Bit().constData());

		ret = QProcess::execute(cmd);

		if (ret == -1 || ret == -2)
		{
			qDebug("[SdObserver::usbAutoUnmount] Failed to umount - Error: [%d]", ret);
			return false;
		}
	}

	// check - read /proc/mounts
	{
		QFile mountFile("/proc/mounts");

		if (mountFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
		{
			qDebug("[SdObserver::usbAutoUnmount] Failed to open '/proc/mounts' file. 2");
			return false;
		}

		mountFileData = mountFile.readAll();

		mountFile.close();

		if (mountFileData.contains("/dev/sd"))
		{
			qDebug("[SdObserver::usbAutoUnmount] Failed to umount - Check Failed");
			return false;
		}
	}
#endif

	qDebug("[SdObserver::usbAutoUnmount] Done. - OK");

	return true;
}
