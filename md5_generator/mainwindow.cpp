#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    this->setWindowTitle("MD5 Generator V0.2");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_file_open_clicked()
{
    static QString prevPath;

	// open
	QString selectedFilter("");
	static const QString filter ("All files (*)");

	m_file_list.clear();
    m_file_list = QFileDialog::getOpenFileNames(this, tr("Open"), prevPath, filter, &selectedFilter);
	if (m_file_list.count() == 0)
		return;

	ui->list_files->clear();
	addFileList();

    prevPath = QDir(m_file_list.at(0)).absolutePath();
}

void MainWindow::addFileList(void)
{
	if(m_file_list.count() != 0)
	{
		ui->list_files->addItems(m_file_list);
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

