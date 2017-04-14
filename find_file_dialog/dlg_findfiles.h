#ifndef DLG_FINDFILES_H
#define DLG_FINDFILES_H

#include <QDialog>

class QDir;

namespace Ui {
class Dlg_FindFiles;
}

class Dlg_FindFiles : public QDialog
{
	Q_OBJECT

public:
	explicit Dlg_FindFiles(QWidget *parent = 0);
	~Dlg_FindFiles();

	void setSearchRoot(QString path);
	void setTitle(const QString &title);

private:
	bool checkDirAvailable(const QString &path);
	void setDefaultSearchRoot();
	int searchFiles();

	int appendFileListFromDirByKeyword(QMap<QString, QStringList> &result, const QDir &dir, const QString &key_word);

	void dumpSearchResult(const QMap<QString, QStringList> &result);

	void updateFileTable(const QMap<QString, QStringList> &result_map);

private slots:
	void on_btn_search_clicked();
	void on_btn_load_clicked();
	void on_btn_close_clicked();

	void on_btn_input_keyword_clicked();

public:
	static const QString EXT_OMA_FILE_DIR;

private:
	Ui::Dlg_FindFiles *ui;

	QString		m_search_root_path;
};

#endif // DLG_FINDFILES_H
