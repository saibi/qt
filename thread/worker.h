#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
	Q_OBJECT
public:
	Worker();
	Worker(QString name);
	~Worker();

signals:
	void finished();
	void progress(QString name, unsigned int count);

public slots:
	void process();

private:
	QString m_name;
	unsigned int m_count;
};

#endif // WORKER_H
