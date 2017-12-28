#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QDir>

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

	connect(ui->treeView, SIGNAL(keyPressed(int)), this, SLOT(slot_keyPressed(int)));
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
	qDebug("[%s]", Q_FUNC_INFO);
	handle_select(index);
}

void MainWindow::on_pushButton_exit_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	close();
}


void MainWindow::on_pushButton_rename_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_rename();
}

void MainWindow::on_pushButton_up_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_up();
}

void MainWindow::slot_keyPressed(int key)
{
	if ( key == Qt::Key_Backspace )
	{
		qDebug("[%s] backspace pressed", Q_FUNC_INFO);
		handle_up();
	}
	else if ( key == Qt::Key_Return || key == Qt::Key_Enter )
	{
		qDebug("[%s] enter pressed", Q_FUNC_INFO);

		QModelIndexList list = ui->treeView->selectionModel()->selectedRows(0);

		if ( list.size() == 1 )
		{			
			handle_select( list.at(0) );
		}
		else if ( list.size() == 2 )
		{
			handle_rename();
		}
	}
}


void MainWindow::on_actionExit_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	close();
}

void MainWindow::on_actionUp_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_up();
}


void MainWindow::on_actionRename_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_rename();
}

void MainWindow::handle_select(const QModelIndex &index)
{
	QFileInfo f = m_dirModel->fileInfo(index);

	if ( f.isDir() )
	{
		QString prev_cwd = m_cwd;

		m_cwd = f.absoluteFilePath();
		qDebug("[%s] directory [%s] selected", Q_FUNC_INFO, qPrintable(f.absoluteFilePath()));

		ui->treeView->setRootIndex(m_dirModel->index(m_cwd) );

		if ( QFileInfo(prev_cwd).dir().absolutePath() == m_cwd )
		{
			ui->treeView->setCurrentIndex(m_dirModel->index(prev_cwd));
			ui->treeView->scrollTo(m_dirModel->index(prev_cwd));
		}
	}
}

void MainWindow::handle_up()
{
	QString prev_cwd = m_cwd;

	QFileInfo f(m_cwd);
	m_cwd = f.dir().absolutePath();
	qDebug("[%s] directory [%s] selected", Q_FUNC_INFO, qPrintable(m_cwd));

	ui->treeView->setRootIndex(m_dirModel->index(m_cwd) );
	ui->treeView->setCurrentIndex(m_dirModel->index(prev_cwd));
	ui->treeView->scrollTo(m_dirModel->index(prev_cwd));
}

bool MainWindow::handle_rename()
{
	QModelIndexList list = ui->treeView->selectionModel()->selectedRows(0);

	qDebug("[%s] %d items selected.", Q_FUNC_INFO, list.size());

	if ( list.size() != 2 )
	{
		QMessageBox::information(this, tr("Error"), tr("Select only 2 files. (video and subtile)"), QMessageBox::Ok);
		return false;
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
		QMessageBox::information(this, tr("Error"), tr("Select video and subtile."), QMessageBox::Ok);
		return false;
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

	return true;
}
