#include "extractor.h"
#include "xnclmessageextractor.h"
#include <QFile>
#include <QMap>
#include <QFileInfo>

Extractor::Extractor(QObject *parent) :
	QObject(parent)
{
}


bool Extractor::extract(const QStringList &inFiles, const QString &outFile, bool checkTr, int contextType, const QString &contextStr)
{
	if ( inFiles.size() <= 0 )
	{
		qDebug("no input files");
		return false;
	}

	if ( outFile.isEmpty() )
	{
		qDebug("need output filename");
		return false;
	}

	if ( contextType == TYPE_STR )
	{
		if ( contextStr.isEmpty() )
		{
			qDebug("empty context string");
			return false;
		}
		return subExtract(inFiles, outFile, checkTr, contextStr);
	}
	else
	{
		return subExtractEx(inFiles, outFile, checkTr, contextType);
	}
}


bool Extractor::subExtract(const QStringList &inFiles, const QString &outFile, bool checkTr, const QString &contextStr)
{
	XnclMessageExtractor	xncl_extractor;
	QStringList				message_list;

	xncl_extractor.setCheckTr( checkTr );

	for(int i = 0 ; i < inFiles.size(); ++i)
	{
		QString file_path = inFiles.at(i);

		if(xncl_extractor.loadXML(file_path))
		{
			QStringList new_list = xncl_extractor.getMessageList();
			for(int j = 0 ; j < new_list.count() ; j++)
			{
				if(message_list.contains(new_list.at(j)) == false)
					message_list.append(xncl_extractor.getMessageList());
			}
		}
	}

	if(message_list.count() == 0)
	{
		qDebug("empty");
		return false;
	}

	return saveMessageTable(message_list, outFile, contextStr);
}


bool Extractor::subExtractEx(const QStringList &inFiles, const QString &outFile, bool checkTr, int type)
{
	XnclMessageExtractor	xncl_extractor;
	QMap < QString, QStringList > map;

	xncl_extractor.setCheckTr( checkTr );

	for(int i = 0 ; i < inFiles.size(); ++i)
	{
		QString file_path = inFiles.at(i);

		if(xncl_extractor.loadXML(file_path))
		{
			map.insert( (type == TYPE_SECTION ) ? xncl_extractor.getSectionString() : QFileInfo(file_path).baseName(), xncl_extractor.getMessageList());
		}
	}

	if ( map.count() <= 0 )
	{
		qDebug("empty");
		return false;
	}

	QFile out_file(outFile);

	if (!out_file.open(QFile::WriteOnly | QFile::Text))
	{
		qDebug("Error: file open [%s]", qPrintable(outFile));
		return false;
	}

	QString data;

	data = "#ifndef _XML_MENU_MESSAGE_H_\n";
	out_file.write(data.toLocal8Bit());
	data = "#define _XML_MENU_MESSAGE_H_\n\n";
	out_file.write(data.toLocal8Bit());

	QList <QString> keyList = map.keys();

	int id = 0;
	for ( int k = 0 ; k < keyList.count(); k++)
	{
		const QStringList message_list = map.value(keyList.at(k));

		for(int i = 0 ; i < message_list.count() ; i++)
		{
			data.sprintf("#define A%04d\tQT_TRANSLATE_NOOP(\"", id++);
			data += "XML_" + keyList.at(k) + "\", \"";
			data += message_list.at(i) + "\")\n";
			out_file.write(data.toLocal8Bit());
		}
	}

	data = "\n#endif";
	out_file.write(data.toLocal8Bit());

	out_file.close();

	return true;
}

bool Extractor::saveMessageTable(const QStringList &message_list, const QString & file_name, const QString & contextStr)
{
	QString out_file_path = file_name;
	QFile out_file(out_file_path);

	if (!out_file.open(QFile::WriteOnly | QFile::Text))
	{
		qDebug("Error: file open [%s]", out_file_path.toLocal8Bit().constData());
		return false;
	}

	QString data;

	data = "#ifndef _XML_MENU_MESSAGE_H_\n";
	out_file.write(data.toLocal8Bit());
	data = "#define _XML_MENU_MESSAGE_H_\n\n";
	out_file.write(data.toLocal8Bit());

	for(int i = 0 ; i < message_list.count() ; i++)
	{
		data.sprintf("#define A%04d\tQT_TRANSLATE_NOOP(\"", i);
		data += contextStr + "\", \"";
		data += message_list.at(i) + "\")\n";
		out_file.write(data.toLocal8Bit());
	}

	data = "\n#endif";
	out_file.write(data.toLocal8Bit());

	out_file.close();
	return true;
}
