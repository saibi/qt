#include "dialog.h"
#include "ui_dialog.h"
#include <QProcess>
#include <QFile>
#include <QDateTime>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
}

Dialog::~Dialog()
{
	delete ui;
}

bool Dialog::createScriptFile(const QString &path, const QString &contents)
{
	QFile f(path);
	if (f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		QTextStream out(&f);

		out << contents;
		f.close();
		f.setPermissions(f.permissions() | QFile::ExeOwner);
		return true;
	}

	return false;
}


bool Dialog::extractGuiBinaryBuildDate(const QString & path, QDate * pDate)
{
	QString script =	"#/bin/bash\n"
						"date_str=`/bin/grep -E '(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec) ( |[[:digit:]])[[:digit:]] 20[[:digit:]][[:digit:]]' -oh -a $1`\n"
						"date -d \"$date_str\" +\"%Y%m%d\"\n";

	if ( !createScriptFile("/tmp/b.sh", script) )
		return false;

	QProcess process;
	QString cmdline;

	cmdline.sprintf("/tmp/b.sh %s", qPrintable(path));
	process.start(cmdline);
	process.waitForFinished();
	cmdline = QString(process.readAllStandardOutput()).simplified();
	if ( !cmdline.isEmpty() )
	{
		qDebug("DBG build [%s]", qPrintable(cmdline));

		QDate date = QDate::fromString(cmdline, "yyyyMMd");
		if ( date.isValid() )
		{
			if ( pDate )
				*pDate = date;

			qDebug("DBG fromString [%s]", qPrintable(date.toString()));
			return true;
		}
	}
	return false;
}

bool Dialog::extractGuiBinaryVersionStr(const QString & path, QString * pVer, bool isEdgerBin)
{
	QString script =	"#/bin/bash\n";

	if ( isEdgerBin )
		script += "/bin/grep nex_edger -a $1 | /bin/grep -a -E '(0|1)\\.[[:digit:]]+\\.[[:digit:]]+[a-z]*' -oh\n";
	else
		script += "/bin/grep \"_[\%1]_[\%2]\" -a $1 | /bin/grep -a -E '(0|1)\\.[[:digit:]]+\\.[[:digit:]]+[a-z]*' -oh\n";


	if ( !createScriptFile("/tmp/b.sh", script) )
		return false;

	QProcess process;
	QString cmdline;

	cmdline.sprintf("/tmp/b.sh %s", qPrintable(path));

	process.start(cmdline);
	process.waitForFinished();
	cmdline = QString(process.readAllStandardOutput()).simplified();
	if ( !cmdline.isEmpty() )
	{
		if ( pVer )
			*pVer = cmdline;

		qDebug("DBG ver [%s]", qPrintable(cmdline));
		return true;
	}
	return false;
}
