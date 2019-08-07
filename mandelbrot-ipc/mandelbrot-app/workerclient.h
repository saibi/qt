#ifndef WORKERCLIENT_H
#define WORKERCLIENT_H

#include <QTcpSocket>
#include <QList>
#include <QDataStream>


#include "jobrequest.h"
#include "jobresult.h"
#include "message.h"

class WorkerClient : public QObject
{
	Q_OBJECT

public:
	WorkerClient(int socketDescriptor);
	int cpuCoreCount() const;

signals:
	void unregistered(WorkerClient * workerClient);
	void jobCompleted(WorkerClient * workerClient, JobResult jobResult);
	void sendJobRequests(QList<JobRequest> requests);

public slots:
	void start();
	void abortJob();

private slots:
	void readMessages();
	void doSendJobRequests(QList<JobRequest> requests);

private:
	void handleWorkerRegistered(Message& message);
	void handleWorkerUnregistered(Message & message);
	void handleJobResult(Message & message);

private:
	int mSocketDescriptor;
	int mCpuCoreCount;
	QTcpSocket mSocket;
	QDataStream mSocketReader;
};

Q_DECLARE_METATYPE(WorkerClient*)

#endif // WORKERCLIENT_H
