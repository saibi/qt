#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFile>

#include <QSettings> //yhkim


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    this->setWindowTitle("MD5 Generator V0.3"); 
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_file_open_clicked()
{
    static QString prevPath;
	const QString rememberPath_KEY("My_default_dir");
	QString testPath;
	QDir CurrentDir;
	QSettings testSettings(QDir::currentPath() + "/my_config_file.ini", QSettings::IniFormat);

	// open
	QString selectedFilter("");
	static const QString filter ("All files (*)");

	if (!m_file_list.isEmpty())
	{
		m_file_list.clear();
		m_file_list = QFileDialog::getOpenFileNames(this, tr("Open"), prevPath, filter, &selectedFilter);
		if (m_file_list.count() == 0)
			return;
	}
	else
	{
		m_file_list.clear();
		
		m_file_list = QFileDialog::getOpenFileNames(this, tr("Open"), testSettings.value(rememberPath_KEY).toString());
//		m_file_list = QFileDialog::getOpenFileNames(this, tr("Open"), testSettings.value(rememberPath_KEY).toString(), filter, &selectedFilter);

		if (m_file_list.count() == 0)
			return;
	}
	
	addFileList();

	prevPath = QDir(m_file_list.at(0)).absolutePath();
	
	testSettings.setValue(rememberPath_KEY, prevPath) ;
	testSettings.sync();

	testPath = testSettings.value(rememberPath_KEY).toString();
	
}

void MainWindow::addFileList(void)
{	
	checkDuplicationFileList();
	if(m_file_list.count() != 0)
	{
		ui->list_files->addItems(m_file_list);
	}
}

void MainWindow::checkDuplicationFileList(void)   //�ߺ� ���õ� ������ ����Ʈ���� ����.
{
	if(NULL != ui->list_files->item(0))
	{
		for (int j = 0 ; j < m_file_list.count() ; j++)
			for (int i = 0 ; i < ui->list_files->count() ; i++)
			{
				if (m_file_list.at(j) == ui->list_files->item(i)->text())
				{ 
					int target_Row = ui->list_files->row( ui->list_files->item(i) );
					ui->list_files->takeItem(target_Row);

					return ;
				}
			}
	}
}

void MainWindow::on_btn_generate_clicked()
{
    int count = 0;

	for(int i = 0 ; i < ui->list_files->count() ; i++)
	{
		QString file_path = ui->list_files->item(i)->text();
		QString out_path = file_path + ".md5";
		QString md5 = getFileMD5(file_path);
        if ( md5.isEmpty() == false )
        {
            saveMD5(out_path, md5);
            count++;
        }
	}
    QMessageBox::question(this, "Information", QString("%1 MD5 file(s) generated.").arg(count), QMessageBox::Ok);
}

void MainWindow::on_btn_del_item_clicked()
{
	int target_Row = ui->list_files->row( ui->list_files->currentItem() );

	ui->list_files->takeItem(target_Row);
	
	QMessageBox::information(this, "Info", "The selected item is removed!");
}


QString MainWindow::getFileMD5(QString path)
{
	QFile file(path);

	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray md5;
		md5 = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
		QString result(md5.toHex());

		qDebug("[%s] MD5 hash = %s", path.toLocal8Bit().constData(), result.toLocal8Bit().constData());
		return result;
	}
	return QString();
}

bool MainWindow::saveMD5(QString path, QString md5)
{
	QFile out_file(path);

	if ( !out_file.open(QFile::WriteOnly | QFile::Text))
	{
		qDebug("[%s] file create error [%s]", Q_FUNC_INFO, path.toLocal8Bit().constData());
		return false;
	}

	out_file.write(md5.toLocal8Bit());
	out_file.close();
	return true;
}

void MainWindow::on_btn_exit_clicked()
{
	close();
}

void MainWindow::on_btn_open_work_directory_clicked()
{
#if defined (Q_OS_WIN32)
	system("explorer .");
#endif
#if defined (Q_OS_LINUX)
	system("/usr/bin/nautilus .");
#endif
}

void MainWindow::on_btn_postfix_clicked()
{
	int count = 0;

	for(int i = 0 ; i < ui->list_files->count() ; i++)
	{
		QString file_path = ui->list_files->item(i)->text();
		QString out_path = file_path + ".ver.bin";
		QByteArray data = makePostfixData(file_path, " monarch:V0.0.0_20190808_postfix:");
		if ( data.isEmpty() == false )
		{
			savePostfixBin(out_path, data);
			count++;
		}
	}
	QMessageBox::question(this, "Information", QString("%1 postfix file(s) generated.").arg(count), QMessageBox::Ok);
}

QByteArray MainWindow::makePostfixData(const QString &path, const QString &postfix)
{
	QFile file(path);

	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray contents = file.readAll();

		contents += postfix.toLocal8Bit();


		for ( int cnt = 4 - (contents.size() % 4); cnt > 0 ; --cnt )
			contents += '\0';

		return contents;
	}
	return QByteArray();
}

bool MainWindow::savePostfixBin(const QString &path, const QByteArray &data)
{
	QFile out_file(path);

	if ( !out_file.open(QFile::WriteOnly))
	{
		qDebug("[%s] file create error [%s]", Q_FUNC_INFO, path.toLocal8Bit().constData());
		return false;
	}

	out_file.write(data);
	out_file.close();
	return true;
}
