#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QObject>

class Extractor : public QObject
{
	Q_OBJECT
public:
	explicit Extractor(QObject *parent = 0);

	enum ExtractType
	{
		TYPE_STR,
		TYPE_SECTION,
		TYPE_FILENAME,
	};

	static bool extract(const QStringList & inFiles, const QString & outFile, bool checkTr, int contextType, const QString & contextStr = "XML_Menu");

signals:

public slots:


private:
	static bool subExtract(const QStringList & inFiles, const QString & outFile, bool checkTr, const QString & contextStr = "XML_Menu");
	static bool subExtractEx(const QStringList &inFiles, const QString &outFile, bool checkTr, int type);
	static bool saveMessageTable(const QStringList &message_list, const QString & file_name, const QString & contextStr);




};

#endif // EXTRACTOR_H
