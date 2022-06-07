#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

	bool createScriptFile(const QString & path, const QString & contents);

	bool extractGuiBinaryBuildDate(const QString & path, QDate *pDate);
	bool extractGuiBinaryVersionStr(const QString & path, QString *pVer, bool isEdgerBin = true);

private:
	Ui::Dialog *ui;
};

#endif // DIALOG_H
