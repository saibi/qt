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

	m_cutMode = false;
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


void MainWindow::on_pushButton_match_clicked()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_match();
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
			handle_match();
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

void MainWindow::on_actionMatch_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_match();
}

void MainWindow::on_action_Delete_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_delete();
}


void MainWindow::on_action_Copy_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_copy(false);
}

void MainWindow::on_actionCut_X_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_copy(true);
}

void MainWindow::on_action_Paste_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	handle_paste();
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

bool MainWindow::handle_match()
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

bool MainWindow::handle_delete()
{
	QModelIndexList list = ui->treeView->selectionModel()->selectedRows(0);
	QString msg;

	if ( list.size() == 1 )
	{
		msg = tr("Do you want to delete %1?").arg(list.at(0).data().toString());
	}
	else if ( list.size() > 1 )
	{
		msg = tr("Do you want to delete %1 files?").arg(list.size());
	}
	else
		return false;

	int answer = QMessageBox::question(this, tr("Confirm"), msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if ( QMessageBox::No == answer)
		return false;

	QString path;

	for (int i = 0; i < list.size(); ++i)
	{
		path = list.at(i).data().toString();

		bool ret = QFile::remove(m_cwd + "/" + path);
		qDebug("Delete %s : %s", qPrintable(path), ret ? "success" : "fail");
	}

	return true;
}

void MainWindow::handle_copy(bool cutMode)
{
	QModelIndexList list = ui->treeView->selectionModel()->selectedRows(0);

	m_clipFileList.clear();
	m_clipDir.clear();

	for (int i = 0 ; i < list.size(); ++i )
	{
		QString fileName = list.at(i).data().toString();
		QFileInfo f(m_cwd + "/" + fileName);
		if ( f.isFile() )
		{
			m_clipFileList.append(fileName);
			qDebug("[%s] File '%s' added to clipboard.", Q_FUNC_INFO, qPrintable(fileName) );
		}
	}

	m_clipDir = m_cwd;
	m_cutMode = cutMode;
}

void MainWindow::handle_paste()
{
	if ( m_clipFileList.size() == 0 )
		return;


	QModelIndexList list = ui->treeView->selectionModel()->selectedRows(0);

	if ( list.size() > 1)
	{
		qDebug("[%s] Select a destination directory only.", Q_FUNC_INFO);
		return;
	}

	QString destDir = m_cwd;

	if ( list.size() == 1 )
	{
		QFileInfo f(m_cwd + "/" + list.at(0).data().toString());

		if ( f.isDir() )
		{
			destDir = f.absoluteFilePath();
		}
	}

	if ( m_clipDir == destDir )
	{
		qDebug("[%s] Select different destination directory.", Q_FUNC_INFO);
		return;
	}

	QMessageBox m(QMessageBox::Information, m_cutMode ? tr("Cut") : tr("Copy"), tr("Please wait..."), QMessageBox::NoButton, 0);
	m.setStandardButtons(0);
	m.show();
	QApplication::processEvents();

	for(int i = 0 ; i < m_clipFileList.size(); ++i )
	{
		QString src = m_clipDir + "/" + m_clipFileList.at(i);
		QString target = destDir + "/" + m_clipFileList.at(i);

		if ( m_cutMode )
		{
			if ( QFile::rename(src, target) )
				qDebug("move : %s -> %s", qPrintable(src), qPrintable(target));
		}
		else
		{
			if ( QFile::copy(src, target) )
				qDebug("copy : %s -> %s", qPrintable(src), qPrintable(target));
		}
	}
	m.hide();
}

