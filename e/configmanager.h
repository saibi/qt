#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QMutex>

class ConfigManager : public QObject
{
	Q_OBJECT

public:
	explicit ConfigManager(QObject * parent = 0);
	~ConfigManager();

	Q_DISABLE_COPY(ConfigManager)

	int getErrorCode() { return m_error; }

public:
	static ConfigManager & instance()
	{
		static ConfigManager manager;
		return manager;
	}

	bool reopen();
	bool setData();
	bool close();

private:
	QMutex _mutex;

	int m_error;
};

#endif // CONFIGMANAGER_H
