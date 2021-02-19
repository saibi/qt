#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>

class QTcpServer;
class QTcpSocket;

class TcpConnection : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

	Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)

	Q_PROPERTY(ConnectionType type READ connectionType WRITE setConnectionType NOTIFY connectionTypeChanged)

public:
	enum ConnectionType { Server, Client };
	Q_ENUMS(ConnectionType)

	TcpConnection(QObject *parent = nullptr);

	ConnectionType connectionType() const;
	void setConnectionType(ConnectionType connectionType);

	int port() const;
	void setPort(int port);

	QString hostName() const;
	void setHostName(const QString &hostName);

public slots:
	void initialize();
	void sendData(const QString &data);

signals:
	void dataReceived(const QString &data);
	void portChanged();
	void hostNameChanged();
	void connectionTypeChanged();

private slots:
	void receivedData();
	void slotConnection();

private:
	int m_port;
	QString m_hostName;
	ConnectionType m_connectionType;

	QTcpServer *m_tcpServer;
	QTcpSocket *m_tcpSocket;

};
#endif
