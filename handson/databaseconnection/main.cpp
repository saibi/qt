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


	{
		QString command = "SELECT name FROM department";
		QSqlQuery query(db);
		if ( query.exec(command) )
		{
			while(query.next())
			{
				QString name = query.value("name").toString();
				qDebug() << name;

			}
		}
	}

	{
		QString command = "SELECT my_user.username, department.name AS deptname FROM (SELECT * FROM user WHERE status = 0) AS my_user INNER JOIN department ON department.id = my_user.deptID";
		QSqlQuery query(db);

		if ( query.exec(command) )
		{
			while(query.next())
			{
				QString username = query.value("username").toString();
				QString department = query.value("deptname").toString();


				qDebug() << username << department;

			}
		}

	}

	return a.exec();
}
