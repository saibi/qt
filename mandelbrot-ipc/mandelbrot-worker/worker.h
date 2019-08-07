#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

#include "message.h"
#include "jobresult.h"

#include "job.h"

class Worker : public QObject
{
	Q_OBJECT
public:
	Worker(QObject * parent = 0);
	~Worker();

	int receivedJobsCounter() const;
	int sentJobsCounter() const;

signals:
	void abortAllJobs();

private slots:
	void readMessages();

private:
	void sendRegister();

	void handleJobRequest(Message & message);
	void handleAllJobsAbort(Message & message);
	void sendJobResult(JobResult jobResult);
	void sendUnregister();

	Job * createJob(const JobRequest & jobRequest);

private:

	QTcpSocket mSocket;

	QDataStream mSocketReader;
	int mReceivedJobsCounter;
	int mSentJobsCounter;
};

#endif // WORKER_H
