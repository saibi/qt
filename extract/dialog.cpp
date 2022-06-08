#include "dialog.h"
#include "ui_dialog.h"
#include <QProcess>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QRegExp>

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


bool Dialog::parseVersionString(const QString &verStr, VERSION_INFO & ver)
{
	QString str;
	QRegExp rx;

	ver.major = ver.minor = ver.patch = 0;
	ver.date.setDate(2014, 4, 16);

	rx.setPattern("\\d+\\.\\d+\\.\\d+");
	if ( rx.indexIn(verStr) > -1 )
	{
		// edger gui/motor (ab gui) version string : 0.0.1_20170105
		str = rx.capturedTexts().at(0);

		ver.major = str.section(".", 0, 0).toInt();
		ver.minor = str.section(".", 1, 1).toInt();
		ver.patch = str.section(".", 2, 2).toInt();

		rx.setPattern("\\d\\d\\d\\d\\d\\d\\d\\d");
		if ( rx.indexIn(verStr) > -1 )
		{
			str = rx.capturedTexts().at(0);
			ver.date = QDate::fromString(str, "yyyyMMdd");
		}
		return true;
	}

	rx.setPattern("[vV]?\\d+\\.\\d+");
	if ( rx.indexIn(verStr) > -1 )
	{
		// ab motor version string : v0.01_161028
		str = rx.capturedTexts().at(0);

		str.remove(QRegExp("[vV]"));
		ver.major = str.section(".", 0, 0).toInt();
		ver.minor = str.section(".", 1, 1).toInt();
		ver.patch = 0;

		rx.setPattern("\\d\\d\\d\\d\\d\\d");
		if ( rx.indexIn(verStr) > -1 )
		{
			str = rx.capturedTexts().at(0);
			ver.date = QDate::fromString(str, "yyMMdd");
			if ( ver.date.year() < 2000 )
				ver.date.setDate( ver.date.year() + 100, ver.date.month(), ver.date.day());
		}
		return true;
	}

	// unknown format
	return false;
}

static bool readEndOfFile(const QString & path, int readBytes, QByteArray & buf)
{
	QFile f(path);

	if ( f.open(QIODevice::ReadOnly) )
	{
		buf.resize(readBytes);

		f.seek( f.size() - readBytes);
		qint64 ret = f.read(buf.data(), readBytes);
		f.close();

		if ( ret > 0 )
			return true;
	}
	return false;
}

#define MAX_BUILD_INFO_LEN 60
#define BUILD_INFO_PREFIX_STR "BUILD:"

bool Dialog::readGuiBinaryBuildInfo(const QString &path, VERSION_INFO & ver)
{
	QByteArray buf;

	// assert : binary file contains build info str "BUILD:HPE910XD_GUI_V1.14.3_20220608.BIN" at the end of the file.

	if ( !readEndOfFile(path, MAX_BUILD_INFO_LEN, buf) )
		return false;

	int idx = buf.indexOf(BUILD_INFO_PREFIX_STR);
	if ( idx < 0 )
		return false;

	return parseVersionString( QString(buf.mid(idx + sizeof(BUILD_INFO_PREFIX_STR) - 1).trimmed()), ver);
}

bool Dialog::readGuiBinaryBuildInfoManual(const QString &path)
{
	VERSION_INFO info;
	QByteArray buf;
	QFile f(path);

	// assert : binary file contains build info str "BUILD:HPE910XD_GUI_V1.14.3_20220608.BIN" at the end of the file.

	if ( f.open(QIODevice::ReadOnly) )
	{
		buf.resize(MAX_BUILD_INFO_LEN);

		f.seek( f.size() - MAX_BUILD_INFO_LEN);
		f.read(buf.data(), MAX_BUILD_INFO_LEN);
		f.close();

	}

	qDebug() << "DBG read:" << path << f.size() << buf.size();

	int idx = buf.indexOf(BUILD_INFO_PREFIX_STR);
	if ( idx > 0 )
	{
		QByteArray buildstr = buf.mid(idx + sizeof(BUILD_INFO_PREFIX_STR) - 1).trimmed();
		qDebug() << "DBG" << buildstr;


		if ( parseVersionString(QString(buildstr), info) )
		{
			qDebug("DBG parseVersionString %d.%d.%d, %s", info.major, info.minor, info.patch, qPrintable(info.date.toString()));
		}


		buildstr.replace(".BIN", "");

		QList <QByteArray> tokens =  buildstr.split('_');
		qDebug() << "DBG" << tokens;

		if ( tokens.size() == 4 )
		{
			// version
			tokens[2].replace("V", "");
			QList <QByteArray> ver = tokens[2].split('.');
			qDebug() << "DBG" << ver;
			if ( ver.size() == 3 )
			{
				info.major = ver[0].toInt();
				info.minor = ver[1].toInt();
				info.patch = ver[2].toInt();
				qDebug() << "DBG" << info.major << info.minor << info.patch;
			}

		}

		// date

		QDate date = QDate::fromString(QString(tokens[3]), "yyyyMMdd");
		if ( date.isValid() )
		{
			info.date = date;
			qDebug() << "DBG" << date;
		}
	}

	return false;
}
