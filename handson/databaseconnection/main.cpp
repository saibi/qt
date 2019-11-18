#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

	db.setHostName("127.0.0.1");
	db.setPort(3306);
	db.setDatabaseName("saibi_db");
	db.setUserName("test");
	db.setPassword("test");

	if ( db.open() )
	{
		qDebug() << "Connected!";
	}
	else
	{
		qDebug() << "Failed to connect.";
		return 0;
	}
	return a.exec();
}
