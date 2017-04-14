#ifndef CMDPROC_H
#define CMDPROC_H

#include <QStringList>

class CmdProc
{
public:
	CmdProc();

	static void usage();
	static QStringList checkInFiles(const QStringList & inFiles);
	static void procCmdLines(const QStringList & list);
};

#endif // CMDPROC_H
