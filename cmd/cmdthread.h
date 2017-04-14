#ifndef CMDTHREAD_H
#define CMDTHREAD_H

#include <QThread>
#include <QString>
#include <QList>

class CmdThread : public QThread
{
	Q_OBJECT
public:
	explicit CmdThread(QObject *parent = 0);

	void setArgs(const QList <QString> &args);
	void startCommand(int id);
	int getProgress();
	int getError();
	
	enum CmdThreadID {
		CTID_TEST,

		CTID_MAX
	};

signals:
	
public slots:
	
protected:
	void run();


private:
	int m_progress;
	QList <QString> m_args;
	int m_id;
	int m_error;

	void testCommand();
};

#endif // CMDTHREAD_H
