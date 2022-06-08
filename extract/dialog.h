#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class Dialog;
}

typedef struct version_info_struct {
	int major;
	int minor;
	int patch;

	QDate date;
} VERSION_INFO;

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

	bool createScriptFile(const QString & path, const QString & contents);

	bool extractGuiBinaryBuildDate(const QString & path, QDate *pDate);
	bool extractGuiBinaryVersionStr(const QString & path, QString *pVer, bool isEdgerBin = true);

	bool parseVersionString(const QString &verStr, VERSION_INFO & ver);
	bool readGuiBinaryBuildInfo(const QString & path, VERSION_INFO & ver);

	bool readGuiBinaryBuildInfoManual(const QString & path);

private:
	Ui::Dialog *ui;
};

#endif // DIALOG_H
