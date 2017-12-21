#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    QSettings settings("saibi_soft", "subtitle_filename_changer");

    m_cwd = settings.value("cwd").toString();
    if (m_cwd.isEmpty() )
        m_cwd = QDir::current().absolutePath();

    int col0_width = settings.value("col0_width").toInt();
    if ( col0_width <= 0 )
        col0_width = 200;

    qDebug("m_cwd [%s]", qPrintable(m_cwd));

	m_dirModel = new QFileSystemModel(this);
	m_dirModel->setRootPath("/");
	m_dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::AllEntries | QDir::Hidden | QDir::NoDot);
	ui->treeView->setModel(m_dirModel);
	ui->treeView->setRootIndex( m_dirModel->index(m_cwd));
    ui->treeView->setColumnWidth(0, col0_width);
}

MainWindow::~MainWindow()
{
	delete ui;

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    QSettings settings("saibi_soft", "subtitle_filename_changer");

    settings.setValue("cwd", m_cwd);
    settings.setValue("col0_width", ui->treeView->columnWidth(0));
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
	qDebug("[%s] start", Q_FUNC_INFO);

	QFileInfo f = m_dirModel->fileInfo(index);

	if ( f.isDir() )
	{
		m_cwd = f.absoluteFilePath();
		qDebug("[%s] %s", Q_FUNC_INFO, qPrintable(f.absoluteFilePath()));

		ui->treeView->setRootIndex(m_dirModel->index(m_cwd) );
	}
}

void MainWindow::on_pushButton_exit_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	close();
}

void MainWindow::on_pushButton_rename_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);

	QModelIndexList list = ui->treeView->selectionModel()->selectedRows(0);

	qDebug("DBG count = %d", list.size());

	if ( list.size() != 2 )
	{
		QMessageBox::information(this, "Error", "Select only 2 files. (video and subtile)", QMessageBox::Ok);
		return;
	}

	QString path;
	QString videoPath;
	QString subPath;

	for (int i = 0; i < list.size(); ++i)
	{
		path = list.at(i).data().toString();

		qDebug("DBG %s", qPrintable(path));
		if ( path.endsWith(".mkv", Qt::CaseInsensitive) ||
			 path.endsWith(".avi", Qt::CaseInsensitive) ||
			 path.endsWith(".mp4", Qt::CaseInsensitive) ||
			 path.endsWith(".mov", Qt::CaseInsensitive) ||
			 path.endsWith(".wmv", Qt::CaseInsensitive) )
		{
			videoPath = path;
		}
		else if ( path.endsWith(".smil", Qt::CaseInsensitive) ||
				  path.endsWith(".smi", Qt::CaseInsensitive) ||
				  path.endsWith(".srt", Qt::CaseInsensitive) )
		{
			subPath = path;
		}
	}

	if ( videoPath.isEmpty() || subPath.isEmpty() )
	{
		QMessageBox::information(this, "Error", "Select video and subtile.", QMessageBox::Ok);
		return;
	}


	QString ext;
	if ( subPath.contains(".srt", Qt::CaseInsensitive ) )
		ext = ".srt";
	else
		ext = ".smi";

	QFileInfo v(videoPath);

	QString newSubPath = v.completeBaseName() + ext;

	qDebug("%s: %s -> %s", qPrintable(m_cwd), qPrintable(subPath), qPrintable(newSubPath));

	QFile::rename( m_cwd + "/" + subPath, m_cwd + "/" + newSubPath);

	ui->treeView->setRootIndex(m_dirModel->index(m_cwd) );
}
