#ifndef SYSINFOMACIMPL_H
#define SYSINFOMACIMPL_H

#include "sysinfo.h"

#include <QtGlobal>
#include <QVector>

class SysInfoMacImpl : public SysInfo
{
public:
	SysInfoMacImpl();

	// SysInfo interface
public:
	void init();
	double cpuLoadAverage();
	double memoryUsed();

private:
	QVector<qulonglong> cpuRawData();

private:
	QVector<qulonglong> mCpuLoadLastValues;
};

#endif // SYSINFOMACIMPL_H
