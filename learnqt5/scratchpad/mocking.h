#ifndef MOCKING_H
#define MOCKING_H

#include <QDebug>

class Client;

class DatabaseController
{
public:
	DatabaseController()
	{
		qDebug() << "Creating a new database connection";

	}

	void save(Client *client)
	{
		Q_UNUSED(client);
		qDebug() << "Saving a Client to the production database";
	}
};

class Client
{
	DatabaseController & databaseController;

public:
	Client(DatabaseController & _databaseController) : databaseController(_databaseController)
	{
	}

	void save()
	{
		qDebug() << "Saving Client";
		databaseController.save(this);
	}
};



#endif // MOCKING_H
