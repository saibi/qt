#include "server.h"
#include <QFile>

Server::Server(QObject *parent) : QObject(parent)
{
	m_socket = 0;
	m_server = new QLocalServer(this);

	QFile::remove("/tmp/tbs.local");

	if ( !m_server->listen("tbs.local") )
	{
		qDebug("[Server::] Unable to start the server: %s", qPrintable(m_server->errorString()));

		delete m_server;
		m_server = 0;
		return;
	}

	connect(m_server, SIGNAL(newConnection()), this, SLOT(slotConnected()));


	m_button = new ButtonDialog[2];

	m_button[0].setId(0);
	m_button[0].move(100, 0);
	m_button[1].setId(1);
	m_button[1].move(200, 0);

	connect(this, SIGNAL(signalCommandReceived(QString)), this, SLOT(slotCommandReceived(QString)));

	qDebug("[Server::] server created");
}


Server::~Server()
{
	delete [] m_button;

	if ( m_socket )
		m_socket->disconnectFromServer();

	delete m_server;
}


void Server::slotConnected()
{
	qDebug("[Server::slotConnected()]");

	if ( m_socket )
	{
		m_socket->disconnectFromServer();
		m_socket = 0;
	}

	m_socket = m_server->nextPendingConnection();

	if ( m_socket )
	{
		connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadReady()));
		connect(m_socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));

		qDebug("[Server::slotConnected()] next socket");
	}
}

void Server::slotReadReady()
{
	if ( !m_socket )
		return;

	QDataStream in(m_socket);
	in.setVersion(QDataStream::Qt_4_7);

	QString cmd;

	in >> cmd;

	qDebug("[Server::slotReadReady()] read [%s]", qPrintable(cmd));

	emit signalCommandReceived(cmd);
}


void Server::slotCommandReceived(const QString &cmd)
{
	static int id = 0;

	QString command = cmd.section(' ', 0, 0);
	QString param = cmd.section(' ', 1, -1);

	qDebug("[Server::slotCommandReceived] id %d, command [%s], param [%s]", id, qPrintable(command), qPrintable(param));

	if ( command.compare("show", Qt::CaseInsensitive) == 0 )
	{
		qDebug("Show");
		m_button[id].show();
	}
	else if ( command.compare("hide", Qt::CaseInsensitive) == 0 )
	{
		qDebug("Hide");
		m_button[id].hide();
	}
	else if ( command.compare("id", Qt::CaseInsensitive) == 0 )
	{
		id = param.toInt();
		if ( id < 0 || id >1 )
			id = 0;
		qDebug("id = %d", id);
	}
	else if ( command.compare("move", Qt::CaseInsensitive) == 0 )
	{
		if ( param.compare("right", Qt::CaseInsensitive) == 0 )
			m_button[id].move( m_button[id].pos().x() + 10, m_button[id].pos().y());
		else if ( param.compare("left", Qt::CaseInsensitive) == 0 )
			m_button[id].move( m_button[id].pos().x() - 10, m_button[id].pos().y());
		else if ( param.compare("up", Qt::CaseInsensitive) == 0 )
			m_button[id].move( m_button[id].pos().x(), m_button[id].pos().y() - 10);
		else if ( param.compare("down", Qt::CaseInsensitive) == 0 )
			m_button[id].move( m_button[id].pos().x(), m_button[id].pos().y() + 10);
		else
		{
			QString x = param.section(',', 0, 0);
			QString y = param.section(',', 1, 1);

			m_button[id].move(x.toInt(), y.toInt());
		}
	}
}
