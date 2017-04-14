#include "nexfiledialog.h"
#include "ui_nexfiledialog.h"

#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QMessageBox>

#include "util/sdobserver.h"
#include "hw/beep.h"
#include "ui/keyboarddlg.h"

NexFileDialog::NexFileDialog(QWidget *parent, const QString &title, const QString &working_dir, const QStringList &ext_filters, const int options) :
	QDialog(parent),
	ui(new Ui::NexFileDialog)
{
	ui->setupUi(this);

	ui->btn_search->setVisible(false);
	ui->table_files->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);

	if(title.isEmpty() == false)
		this->setTitle(title);
	setWorkingDir(working_dir);
	setExtFilters(ext_filters);
	m_selected_file_path = QString();
	m_enable_sub_dir_search = false;

	applyOptions(options);
}

NexFileDialog::~NexFileDialog()
{
	delete ui;
}

void NexFileDialog::setWorkingDir(QString path)
{
	QFileInfo path_info(path);

	if(path_info.exists())
	{
		if(path_info.isDir())
		{
			qDebug("[NexFileDialog::setWorkingDir] set working dir: %s", qPrintable(path));
			m_working_dir_path = path;
			return;
		}

		qDebug("[NexFileDialog::setWorkingDir] invalid path ('%s' is not directory).", qPrintable(path));
	}
	else
	{
		qDebug("[NexFileDialog::setWorkingDir] '%s' is not exist.", qPrintable(path));
	}

	m_working_dir_path.clear();
	return;
}

void NexFileDialog::setExtFilters(const QStringList &ext_filters)
{
	m_ext_filters = ext_filters;
}

QString NexFileDialog::getSelectedFilePath()
{
	return m_selected_file_path;
}

void NexFileDialog::setTitle(const QString &title)
{
	this->setWindowTitle(title);
}

void NexFileDialog::applyOptions(int options)
{
	// current default of .ui is search enabled
	if(!(options & EnableSearch))
		ui->widget_search->setVisible(false);
	else
	{
		if(options & EnableSubDirSearch)
			m_enable_sub_dir_search = true;
	}

	if(!(options & HideWorkingDirFilesAtInit))
		searchFiles(false);
}

bool NexFileDialog::checkDirAvailable(const QString &path)
{
	if(path.isEmpty())
		return false;

	QDir dir = QDir(path);

	if(dir.exists())
		return true;

	return false;
}

int NexFileDialog::searchFiles(bool include_sub_dir)
{
	if(checkDirAvailable(m_working_dir_path) == false)
	{
		return -1;
	}

	QDir sub_dir;
	QString search_key = ui->btn_input_keyword->text();
	QMap<QString, QStringList> search_result;

	// search current directory
	sub_dir = QDir(m_working_dir_path);
	appendFileListFromDirByKeyword(search_result, sub_dir, search_key);

	if(include_sub_dir)
	{
		// search sub directory
		QDirIterator dir_it(m_working_dir_path, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		while(dir_it.hasNext())
		{
			dir_it.next();

			sub_dir = QDir(dir_it.filePath());
			appendFileListFromDirByKeyword(search_result, sub_dir, search_key);
		}
	}

	updateFileTable(search_result);

	return search_result.size();
}

int NexFileDialog::appendFileListFromDirByKeyword(QMap<QString, QStringList> &result, const QDir &dir, const QString &key_word)
{
	QString sub_dir_path = dir.absolutePath();
	sub_dir_path.remove(m_working_dir_path);

	QStringList file_list;

#ifndef Q_OS_WIN
	file_list.append(dir.entryList(getNameFilterList(key_word), QDir::Files));
#else
	file_list.append(dir.entryList(getNameFilterList(key_word), QDir::Files));
#endif

//	qDebug("[NexFileDialog::appendFileListFromDirByKeyword] dir: %s (found:%d)", qPrintable(dir.absolutePath()), file_list.size());
	if(file_list.isEmpty() == false)
	{
		result.insert(sub_dir_path, file_list);
	}

//	dumpSearchResult(result);
	return file_list.size();
}

QStringList NexFileDialog::getNameFilterList(const QString &search_key_word)
{
	QString name_str;
	QStringList name_filters;

	name_str = (search_key_word.isEmpty())? "*" : "*" + search_key_word + "*";

	if(m_ext_filters.isEmpty())
	{
		name_filters << name_str + ".*";
	}
	else
	{
		for(int i = 0 ; i < m_ext_filters.size() ; i++)
		{
			name_filters << name_str + m_ext_filters.at(i);
		}
	}

	return name_filters;
}

void NexFileDialog::dumpSearchResult(const QMap<QString, QStringList> &result)
{
	if(result.isEmpty() == false)
	{
		QStringList file_list;

		qDebug("[NexFileDialog::dumpSearchResult] dump searching result ====================");
		QMap<QString, QStringList>::const_iterator result_itr = result.constBegin();
		while(result_itr != result.constEnd())
		{
			file_list = result_itr.value();
			for(int i = 0 ; i < file_list.size() ; i++)
			{
				qDebug("\t(%s)    %04d:%s", qPrintable(result_itr.key()), i+1, qPrintable(file_list.at(i)));
			}
			result_itr++;
		}
	}
}

void NexFileDialog::updateFileTable(const QMap<QString, QStringList> &result_map)
{
	ui->table_files->clear();
	ui->table_files->setRowCount(0);

	if(result_map.isEmpty() == false)
	{
		QStringList file_list;
		int row = 0;

		QMap<QString, QStringList>::const_iterator result_itr = result_map.constBegin();
		while(result_itr != result_map.constEnd())
		{
			file_list = result_itr.value();
			for(int i = 0 ; i < file_list.size() ; i++)
			{
				QTableWidgetItem *file_name_item = new QTableWidgetItem(result_itr.key() + "/" + file_list.at(i));
				ui->table_files->insertRow(row);
				ui->table_files->setItem(row, 0, file_name_item);
				row++;
			}
			result_itr++;
		}
	}
}

void NexFileDialog::on_btn_search_clicked()
{
	Beep::beepClick();

	searchFiles(m_enable_sub_dir_search);
}

void NexFileDialog::on_btn_select_clicked()
{
	Beep::beepClick();

	if(ui->table_files->rowCount() > 0)
	{
		int cur_row = ui->table_files->currentRow();
		if(cur_row >= 0)
		{
			QString file_name = ui->table_files->item(cur_row, 0)->text();

			QString load_file_path = m_working_dir_path + file_name;

			m_selected_file_path = load_file_path;
			qDebug("[NexFileDialog::on_btn_select_clicked] selected file: %s", qPrintable(load_file_path));
			accept();
		}
		else
		{
			Beep::beepWarning();
			QMessageBox::warning(this, this->windowTitle(), tr("Select the file from Table."));
			Beep::beepClick();
		}
	}
	else
	{
		Beep::beepWarning();
		QMessageBox::warning(this, this->windowTitle(), tr("Empty List. Search First."));
		Beep::beepClick();
	}
}

void NexFileDialog::on_btn_close_clicked()
{
	Beep::beepClick();

	reject();
}

void NexFileDialog::on_btn_input_keyword_clicked()
{
	Beep::beepClick();

	KeyboardDlg dlg(this);

	dlg.setWindowTitle(tr("Input Search String"));
	dlg.setMaxLength(32);

	if(dlg.exec() == QDialog::Accepted)
	{
		ui->btn_input_keyword->setText(dlg.getText().trimmed());
		searchFiles(m_enable_sub_dir_search);
	}
}


QString NexFileDialog::getOpenFileName(QWidget *parent,
									   const QString &title,
									   const QString &working_dir,
									   const QStringList &ext_filters
									   )
{
	// create a dialog
	NexFileDialog dlg(parent, title, working_dir, ext_filters);
	if(dlg.exec() == QDialog::Accepted)
	{
		return dlg.getSelectedFilePath();
	}
	return QString();
}

QString NexFileDialog::getOpenFileNameWithSearch(QWidget *parent,
												 const QString &title,
												 const QString &working_dir,
												 const QStringList &ext_filters,
												 int options
												 )
{
	if(options == 0)
		options = EnableSearch | EnableSubDirSearch | HideWorkingDirFilesAtInit; // default option for searching

	if(!(options & EnableSearch)) options |= EnableSearch;

	// create a dialog
	NexFileDialog dlg(parent, title, working_dir, ext_filters, options);
	if(dlg.exec() == QDialog::Accepted)
	{
		return dlg.getSelectedFilePath();
	}
	return QString();
}
