#include "cmdproc.h"
#include <QDebug>
#include "version.h"
#include <QFileInfo>
#include <QDir>
#include "extractor.h"


CmdProc::CmdProc()
{
}

void CmdProc::usage()
{
	qDebug(TITLE_STR " " VERSION_STR);
	qDebug("xncl_extract [--tr] [--section|--filename|--context ContextString] [--out OutFileName] [FileName | DirectoryName ...]");
	qDebug("	--tr : check tr");
	qDebug("	--section : use section name as context string");
	qDebug("	--filename : use filename as context string");
	qDebug("	--context ContextString : set context string");
	qDebug("	--out OutFileName : set output filename");
	qDebug("");
}


QStringList CmdProc::checkInFiles(const QStringList & inFiles)
{
	QStringList files;

	QStringList filters;
	filters << "*.xml" << "*.XML";

	for (int i = 0 ; i < inFiles.size(); ++i )
	{
		QFileInfo info(inFiles.at(i));

		if ( info.isFile() )
			files += inFiles.at(i);
		else if (info.isDir())
		{
			QDir dr(inFiles.at(i));
			QStringList list = dr.entryList(filters, QDir::Files);

			for ( int k = 0; k < list.size(); k++ )
				files.append( QString(inFiles.at(i) + "/" + list.at(k)));
		}
	}
	return files;
}

void CmdProc::procCmdLines(const QStringList & list)
{
	// parse command line args
	bool contextOpt = false;
	int type = Extractor::TYPE_STR;
	QString outFile;
	QStringList inList;
	bool checkTr = false;
	QString contextStr;

	for (int i = 1 ; i < list.size(); ++i )
	{
		if ( list.at(i).compare("--help") == 0 )
		{
			usage();
			return;
		}
		else if ( contextOpt == false && list.at(i).compare("--section") == 0 )
		{
			contextOpt = true;
			type = Extractor::TYPE_SECTION;
		}
		else if ( contextOpt == false && list.at(i).compare("--filename") == 0 )
		{
			contextOpt = true;
			type = Extractor::TYPE_FILENAME;
		}
		else if ( outFile.isEmpty() && list.at(i).compare("--out") == 0 )
		{
			++i;
			outFile = list.at(i);
		}
		else if ( list.at(i).compare("--tr") == 0 )
		{
			checkTr = true;
		}
		else if ( contextStr.isEmpty() && list.at(i).compare("--context") == 0 )
		{
			++i;
			contextStr = list.at(i);
		}
		else
		{
			inList += list.at(i);
		}
	}
	if ( contextStr.isEmpty() )
		contextStr = "XML_Menu";

	if ( outFile.isEmpty() )
		outFile = "xml_menu_message.h";

	if ( inList.size() < 1 )
	{
		usage();
		return;
	}

	QStringList inFiles = checkInFiles(inList);


	for (int i = 0 ; i < inFiles.size(); ++i)
		qDebug("[%s]", qPrintable(inFiles.at(i)));


	if ( Extractor::extract(inFiles, outFile, checkTr, type, contextStr ) )
		qDebug("%s extracted.", qPrintable(outFile));
}
