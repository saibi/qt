#include "buttoncontroller.h"
#include <QFile>
#include <QProcess>
#include <QMutex>

static QMutex g_mutex;

#define DEFAULT_NAME "ibizabtn"

ButtonController::ButtonController(QObject *parent) : QObject(parent)
{
	QMutexLocker locker(&g_mutex);

	m_socket = new QLocalSocket(this);
	Q_CHECK_PTR(m_socket);
	if ( ! m_socket )
	{
		qDebug("[ButtonController::] socket alloc error");
		return;
	}

	connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(slotSocketError(QLocalSocket::LocalSocketError)));
	connect(m_socket, SIGNAL(connected()), this, SLOT(slotConnected()));

	m_connected = false;
	m_error = 0;
}

ButtonController::~ButtonController()
{
	disconnectFromServer();
	delete m_socket;
}

void ButtonController::slotSocketError(QLocalSocket::LocalSocketError socketError)
{
	m_error = socketError;

#if 0
	switch (socketError) {
	case QLocalSocket::ServerNotFoundError:
		qDebug("[ButtonController::slotSocketError] The host was not found. Please check the host name and port settings.");
		break;

	case QLocalSocket::ConnectionRefusedError:
		qDebug("[ButtonController::slotSocketError] The connection was refused by the peer. Make sure the server is running, and check that the host name and port settings are correct.");
		break;

	case QLocalSocket::PeerClosedError:
		qDebug("[ButtonController::slotSocketError] Peer closed");
		break;

	default:
		qDebug("[ButtonController::slotSocketError] The following error occurred: %s", qPrintable(m_socket->errorString()));
	}
#endif
}

void ButtonController::slotConnected()
{
	qDebug("[ButtonController::slotConnected]");
	m_connected = true;
	m_error = 0;

	connect(m_socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

void ButtonController::slotDisconnected()
{
	qDebug("[ButtonController::slotDisconnected]");
	m_connected = false;

	emit signalDisconnected();
}

bool ButtonController::connectToServer(const QString &name)
{
	if ( !m_socket )
	{
		qDebug("[ButtonController::connect] socket is not allocated.");
		return false;
	}

	disconnectFromServer();

	QMutexLocker locker(&g_mutex);
	m_socket->connectToServer( name.isEmpty() ? DEFAULT_NAME : name, QIODevice::WriteOnly);
	if ( m_socket->waitForConnected(1 * 1000))
		return true;
	return false;
}

void ButtonController::disconnectFromServer()
{

	if ( m_connected && m_socket )
	{
		QMutexLocker locker(&g_mutex);

		m_socket->abort();
		//m_connected = false;
	}
}

void ButtonController::sendCommand(const QString &cmd)
{
	if ( m_connected && m_socket )
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);

		out.setVersion(QDataStream::Qt_4_7);
		out << cmd;

		QMutexLocker locker(&g_mutex);

		m_socket->write(block);
		m_socket->flush();
		qDebug("[ButtonController::sendCommand] send [%s]", qPrintable(cmd));
	}
}

bool ButtonController::launchServer(const QString & serverPath)
{
	if(QFile::exists(serverPath) == false)
	{
		qDebug("[ButtonController::launchServer] [%s] file not found.", qPrintable(serverPath));
		return false;
	}

	QMutexLocker locker(&g_mutex);

#ifdef __linux__
	QString cmd = "pkill -f " + serverPath;
	QProcess::execute("cmd");
#endif

	if(QProcess::startDetached(serverPath) == false)
	{
		qDebug("[ButtonController::launchServer] [%s] launching error.", qPrintable(serverPath));
		return false;
	}
	return true;

#if 0
	if ( !m_serverProcess.pid() )
	{
		qDebug("[ButtonController::launchServer] start server [%s]", qPrintable(serverPath));
		m_serverProcess.start(serverPath);
		if ( m_serverProcess.waitForStarted(3000) )
			qDebug("[ButtonController::launchServer] server started.");
		else
		{
			qDebug("[ButtonController::launchServer] server start error.");
			return false;
		}
	}
	else
	{
		qDebug("[ButtonController::launchServer] server already running. pid = %d", int(m_serverProcess.pid()));
	}
	return true;
#endif

}

#if 0
bool ButtonController::terminateServer()
{
	if ( m_serverProcess.pid() )
	{
		m_serverProcess.kill();
		return true;
	}
	return false;
}
#endif
