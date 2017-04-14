#include "controller.h"
#include <QFile>
#include <QProcess>

Controller::Controller(QObject *parent) : QObject(parent)
{

	m_socket = new QLocalSocket(this);

	if ( ! m_socket )
	{
		qDebug("[Controller::] socket alloc error");
		return;
	}

	connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(slotSocketError(QLocalSocket::LocalSocketError)));
	connect(m_socket, SIGNAL(connected()), this, SLOT(slotConnected()));

	m_connected = false;
}

Controller::~Controller()
{
	disconnectFromServer();
	delete m_socket;
}

void Controller::slotSocketError(QLocalSocket::LocalSocketError socketError)
{
	switch (socketError) {
	case QLocalSocket::ServerNotFoundError:
		qDebug("[Controller::slotSocketError] The host was not found. Please check the host name and port settings.");
		break;

	case QLocalSocket::ConnectionRefusedError:
		qDebug("[Controller::slotSocketError] The connection was refused by the peer. Make sure the server is running, and check that the host name and port settings are correct.");
		break;

	case QLocalSocket::PeerClosedError:
		qDebug("[Controller::slotSocketError] Peer closed");
		break;

	default:
		qDebug("[Controller::slotSocketError] The following error occurred: %s", qPrintable(m_socket->errorString()));
	}
}

void Controller::slotConnected()
{
	qDebug("[Controller::slotConnected]");
	m_connected = true;
}

void Controller::connectToServer()
{
	if ( !m_socket )
	{
		qDebug("[Controller::connect] socket is not allocated.");
		return;
	}

	disconnectFromServer();
	m_socket->connectToServer("tbs.local", QIODevice::WriteOnly);
}

void Controller::disconnectFromServer()
{
	if ( m_connected && m_socket )
	{
		m_socket->abort();
		m_connected = false;
	}
}

void Controller::sendCommand(const QString &cmd)
{
	if ( m_connected && m_socket )
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);

		out.setVersion(QDataStream::Qt_4_7);
		out << cmd;

		m_socket->write(block);
		m_socket->flush();
		qDebug("[Controller::sendCommand] send [%s]", qPrintable(cmd));
	}
}

bool Controller::launchServer(const QString & serverPath)
{
	if(QFile::exists(serverPath) == false)
	{
		qDebug("[%s] file not found.", qPrintable(serverPath));
		return false;
	}

	QString cmd = "pkill -f " + serverPath;
	QProcess::execute("cmd");

	if(QProcess::startDetached(serverPath) == false)
	{
		qDebug("server launching error.");
		return false;
	}
	return true;
}
