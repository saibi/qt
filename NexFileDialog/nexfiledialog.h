#ifndef __NEXFILEDIALOG_H__
#define __NEXFILEDIALOG_H__

#include <QDialog>

class QDir;

namespace Ui {
class NexFileDialog;
}

class NexFileDialog : public QDialog
{
	Q_OBJECT

public:
	enum Option
	{
		EnableSearch					= 0x0001,
		EnableSubDirSearch				= 0x0002,
		HideWorkingDirFilesAtInit		= 0x0004,
	};

	explicit NexFileDialog(QWidget *parent = 0,
						   const QString &title = QString(),
						   const QString &working_dir = QString(),
						   const QStringList &ext_filters = QStringList(),
						   const int options = 0
			);
	~NexFileDialog();

	void setWorkingDir(QString path);
	void setTitle(const QString &title);
	void setExtFilters(const QStringList &ext_filters);

	QString getSelectedFilePath();

private:
	void applyOptions(int options);
	bool checkDirAvailable(const QString &path);
	int searchFiles(bool include_sub_dir = true);

	int appendFileListFromDirByKeyword(QMap<QString, QStringList> &result, const QDir &dir, const QString &key_word);

	QStringList getNameFilterList(const QString &search_key_word = QString());

	void dumpSearchResult(const QMap<QString, QStringList> &result);

	void updateFileTable(const QMap<QString, QStringList> &result_map);

private slots:
	void on_btn_search_clicked();
	void on_btn_select_clicked();
	void on_btn_close_clicked();

	void on_btn_input_keyword_clicked();

private:
	Ui::NexFileDialog *ui;

	QString			m_working_dir_path;
	QStringList		m_ext_filters;
	QString			m_selected_file_path;
	bool			m_enable_sub_dir_search;


public:
	static QString getOpenFileName(QWidget *parent = 0,
								   const QString &title = QString(),
								   const QString &working_dir = QString(),
								   const QStringList &ext_filters = QStringList()
			);

	static QString getOpenFileNameWithSearch(QWidget *parent = 0,
											 const QString &title = QString(),
											 const QString &working_dir = QString(),
											 const QStringList &ext_filters = QStringList(),
											 int options = 0);
};

#endif // __NEXFILEDIALOG_H__
