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

class ClientTestSuite
{
public:
	void saveTests()
	{

		DatabaseController databaseController;
		Client client1(databaseController);
		client1.save();
		Client client2(databaseController);
		client2.save();

		qDebug() << "Test passed!";
	}
};


#endif // MOCKING_H
