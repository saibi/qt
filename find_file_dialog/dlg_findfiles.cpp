#include "dlg_findfiles.h"
#include "ui_dlg_findfiles.h"

#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QMessageBox>

#include "util/sdobserver.h"
#include "hw/beep.h"
#include "ui/keyboarddlg.h"

#define TBL_COLUMN_FILE_NAME	0
#define TBL_COLUMN_FILE_DIR		1

const QString Dlg_FindFiles::EXT_OMA_FILE_DIR = "/ext_oma_files";

Dlg_FindFiles::Dlg_FindFiles(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dlg_FindFiles)
{
	m_search_root_path = "";

	ui->setupUi(this);

	ui->btn_search->setVisible(false);

	ui->table_files->horizontalHeader()->setResizeMode(TBL_COLUMN_FILE_NAME, QHeaderView::Stretch);
#ifdef Q_OS_WIN
//	ui->table_files->verticalHeader()->setVisible(true);
#endif
}

Dlg_FindFiles::~Dlg_FindFiles()
{
	delete ui;
}

void Dlg_FindFiles::setSearchRoot(QString path)
{
	QFileInfo path_info(path);

	if(path_info.exists())
	{
		if(path_info.isDir() == false)
		{
			setDefaultSearchRoot();
			qDebug("[Dlg_FindFiles::setSearchRoot] invalid path ('%s' is not directory). set default search root.", qPrintable(path));
			return;
		}

		qDebug("[Dlg_FindFiles::setSearchRoot] set search root: %s", qPrintable(path));

		m_search_root_path = path;
	}
	else
	{
		qDebug("[Dlg_FindFiles::setSearchRoot] '%s' is not exist.", qPrintable(path));
	}
}

void Dlg_FindFiles::setTitle(const QString &title)
{
	this->setWindowTitle(title);
}

bool Dlg_FindFiles::checkDirAvailable(const QString &path)
{
	if(path.isEmpty())
		return false;

	QDir dir = QDir(path);

	if(dir.exists())
		return true;

	return false;
}

void Dlg_FindFiles::setDefaultSearchRoot()
{
	setSearchRoot(SdObserver::instance().getMountPath()+EXT_OMA_FILE_DIR);
}

int Dlg_FindFiles::searchFiles()
{
	if(checkDirAvailable(m_search_root_path) == false)
	{
		setDefaultSearchRoot();
		if(checkDirAvailable(m_search_root_path) == false)
		{
			return -1;
		}
	}

	QDir sub_dir;
	QString search_key = ui->btn_input_keyword->text();
	QMap<QString, QStringList> search_result;

	// search current directory
	sub_dir = QDir(m_search_root_path);
	appendFileListFromDirByKeyword(search_result, sub_dir, search_key);

	// search sub directory
	QDirIterator dir_it(m_search_root_path, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while(dir_it.hasNext())
	{
		dir_it.next();

		sub_dir = QDir(dir_it.filePath());
		appendFileListFromDirByKeyword(search_result, sub_dir, search_key);
	}

	updateFileTable(search_result);

	return search_result.size();
}

int Dlg_FindFiles::appendFileListFromDirByKeyword(QMap<QString, QStringList> &result, const QDir &dir, const QString &key_word)
{
	QString sub_dir_path = dir.absolutePath();
	sub_dir_path.remove(m_search_root_path);

	QStringList file_list;

//#ifndef Q_OS_WIN
//	file_list.append(dir.entryList(QStringList() << "*" + key_word + "*.oma" << "*" + key_word + "*.OMA", QDir::Files));
//#else
	file_list.append(dir.entryList(QStringList("*.*"), QDir::Files));
//#endif

//	qDebug("[Dlg_FindFiles::appendFileListFromDirByKeyword] dir: %s (found:%d)", qPrintable(dir.absolutePath()), file_list.size());
	if(file_list.isEmpty() == false)
	{
		result.insert(sub_dir_path, file_list);
	}

//	dumpSearchResult(result);
	return file_list.size();
}

void Dlg_FindFiles::dumpSearchResult(const QMap<QString, QStringList> &result)
{
	if(result.isEmpty() == false)
	{
		QStringList file_list;

		qDebug("[Dlg_FindFiles::dumpSearchResult] dump searching result ====================");
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

void Dlg_FindFiles::updateFileTable(const QMap<QString, QStringList> &result_map)
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
				QTableWidgetItem *file_name_item = new QTableWidgetItem(file_list.at(i));
				QTableWidgetItem *file_dir_item = new QTableWidgetItem(result_itr.key());
				ui->table_files->insertRow(row);
				ui->table_files->setItem(row, TBL_COLUMN_FILE_NAME, file_name_item);
				ui->table_files->setItem(row, TBL_COLUMN_FILE_DIR, file_dir_item);
				row++;
			}
			result_itr++;
		}
	}
}

void Dlg_FindFiles::on_btn_search_clicked()
{
	Beep::beepClick();

	searchFiles();
}

void Dlg_FindFiles::on_btn_load_clicked()
{
	Beep::beepClick();

	if(ui->table_files->rowCount() > 0)
	{
		int cur_row = ui->table_files->currentRow();
		if(cur_row >= 0)
		{
			QString file_name = ui->table_files->item(cur_row, TBL_COLUMN_FILE_NAME)->text();
			QString dir_name = ui->table_files->item(cur_row, TBL_COLUMN_FILE_DIR)->text();

			QString load_file_path = m_search_root_path + dir_name + "/" + file_name;

			QMessageBox::information(this, this->windowTitle(), "Selected file: " + load_file_path);
			qDebug("[Dlg_FindFiles::on_btn_load_clicked] selected file: %s", qPrintable(load_file_path));
		}
		else
		{
			Beep::beepWarning();
			QMessageBox::warning(this, this->windowTitle(), tr("Select the file to load."));
			Beep::beepClick();
		}
	}
	else
	{
		Beep::beepWarning();
		QMessageBox::warning(this, this->windowTitle(), tr("Load must be performed after searching."));
		Beep::beepClick();
	}
}

void Dlg_FindFiles::on_btn_close_clicked()
{
	Beep::beepClick();

	reject();
}

void Dlg_FindFiles::on_btn_input_keyword_clicked()
{
	Beep::beepClick();

	KeyboardDlg dlg(this);

	dlg.setWindowTitle(tr("Input Search String"));
	dlg.setMaxLength(32);

	if(dlg.exec() == QDialog::Accepted)
	{
		ui->btn_input_keyword->setText(dlg.getText().trimmed());
		searchFiles();
	}
}
