#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>

#include "buttondialog.h"

class Server : public QObject
{
	Q_OBJECT
public:
	explicit Server(QObject *parent = 0);
	~Server();

signals:
	void signalCommandReceived(const QString & cmd);

public slots:


private slots:
	void slotConnected();
	void slotReadReady();

	void slotCommandReceived(const QString & cmd);

private:
	QLocalServer * m_server;
	QLocalSocket * m_socket;

	ButtonDialog *m_button;
};

#endif // SERVER_H
