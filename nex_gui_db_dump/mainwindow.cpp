#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_Open_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);

	QString filePath = QFileDialog::getOpenFileName(this, "Open db File", m_prevFile, "db files (*.cfg);;All files (*.*)");

	if ( filePath.isEmpty() )
	{
		qDebug("[%s] no file selected. canceled.", Q_FUNC_INFO);
		return;
	}
	qDebug("[%s] open %s.", Q_FUNC_INFO, qPrintable(filePath));

	dumpDb(filePath);

	m_prevFile = filePath;
}

void MainWindow::on_actionE_xit_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	this->close();
}

#define CONFIG_CONNECTION_NAME "config"

/// read all config data from db file
bool MainWindow::loadConfigDBAll(const QString & path, QList<FullConfigData> & dataList)
{
	bool ret = true;;

	if (QSqlDatabase::contains(CONFIG_CONNECTION_NAME) == false)
		QSqlDatabase::addDatabase(QString("QSQLITE"), CONFIG_CONNECTION_NAME);

	{
		QSqlDatabase db = QSqlDatabase::database(CONFIG_CONNECTION_NAME, false);

		if (db.isOpen())
			db.close();

		db.setDatabaseName(path);
		if ( db.open())
		{
			QSqlQuery query(db);

			query.prepare("SELECT section, id, data FROM config_data");

			if (query.exec())
			{
				FullConfigData item;

				while (query.next())
				{
					item.section = query.value(0).toString();
					item.id = query.value(1).toString();
					item.value = query.value(2).toString();
					dataList.append(item);
				}
			}
			else
			{
				qDebug("[ConfigManager::loadConfigDBAll] Error: DB - SELECT [%s][%s]", qPrintable(query.lastError().driverText()), qPrintable(query.lastError().databaseText()));
				ret = false;
			}
			query.clear();
			db.close();
		}
		else
		{
			qDebug("[ConfigManager::loadConfigDBAll] Error: DB - OPEN database file.");
			ret = false;
		}
	}

	QSqlDatabase::removeDatabase(CONFIG_CONNECTION_NAME);
	return ret;
}


void MainWindow::dumpDb(const QString & filePath)
{
	QList<FullConfigData> list;

	if ( loadConfigDBAll(filePath, list) )
	{
		qDebug("%d items", list.size());
		for (int i = 0 ; i < list.size() ; ++i)
		{
			qDebug("%s %s %s", qPrintable(list.at(i).section), qPrintable(list.at(i).id), qPrintable(list.at(i).value));
		}
	}
}
