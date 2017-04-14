#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QLocalSocket>

class Controller : public QObject
{
	Q_OBJECT
public:
	explicit Controller(QObject *parent = 0);
	~Controller();

	static Controller & instance()
	{
		static Controller control;
		return control;
	}

	void connectToServer();
	void disconnectFromServer();
	void sendCommand(const QString & cmd);
	bool launchServer(const QString & serverPath);

signals:

private slots:
	void slotSocketError(QLocalSocket::LocalSocketError socketError);
	void slotConnected();

private:
	QLocalSocket * m_socket;

	bool m_connected;
};

#endif // CONTROLLER_H
