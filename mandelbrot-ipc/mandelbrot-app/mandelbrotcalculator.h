#ifndef MANDELBROTCALCULATOR_H
#define MANDELBROTCALCULATOR_H

#include <memory>
#include <vector>

#include <QTcpServer>
#include <QList>
#include <QThread>
#include <QMap>
#include <QElapsedTimer>

#include "workerclient.h"
#include "jobresult.h"
#include "jobrequest.h"

class MandelbrotCalculator : public QTcpServer
{
	Q_OBJECT

public:
	MandelbrotCalculator(QObject * parent = 0);
	~MandelbrotCalculator();

signals:
	void pictureLinesGenerated(QList<JobResult> jobResults);
	void abortAllJobs();

public slots:
	void generatePicture(QSize areaSize, QPointF moveOffset, double scaleFactor, int iterationMax);

private slots:
	void process(WorkerClient * workerClient, JobResult jobResult);
	void removeWorkerClient(WorkerClient * workerClient);

protected:
	void incomingConnection(qintptr socketDescriptor) override;

private:
	std::unique_ptr<JobRequest> createJobRequest(int pixelPOsitionY);
	void sendJobRequests(WorkerClient & client, int jobRequestCount = 1);
	void clearJobs();

private:
	QPointF mMoveOffset;
	double mScaleFactor;
	QSize mAreaSize;
	int mIterationMax;
	int mReceivedJobResults;
	QList<JobResult> mJobResults;
	QMap<WorkerClient *, QThread*> mWorkerClients;
	std::vector<std::unique_ptr<JobRequest>> mJobRequests;
	QElapsedTimer mTimer;
};

#endif // MANDELBROTCALCULATOR_H
