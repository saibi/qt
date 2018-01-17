#ifndef CAMTHREAD_H
#define CAMTHREAD_H

#include <QThread>

#include <QImage>
#include <QMutex>
#include <QWaitCondition>

#define CAM_SIZE_NORMAL     480
#define CAM_SIZE_BIG        960

#define CAM_SIZE_x1_0		480
#define CAM_SIZE_x2_0		960

#define CAM_STREAM_FRAMES  4 /* Number of streaming buffer */

class CamThread : public QThread
{
	Q_OBJECT

private:
	explicit CamThread(QObject *parent = 0);

	Q_DISABLE_COPY(CamThread)
public:
	enum CameraIndex
	{
		ImageCamera = 0,
		NoSelectedCamera = 5,
	};

	enum MirrorMode
	{
		NoMirror = 0,
		RowMirror = 1,
		ColumnMirror = 2
	};

	static CamThread& instance()
	{
		static CamThread cam_thread;
		return cam_thread;
	}

	bool startCam(int cam_size = CAM_SIZE_x1_0);
	void stopCam();

	int getCurrentCameraSize();

	bool isRunning();


protected:
	void run();

	int initCam(int mirrorMode = NoMirror);

	void runNormalCamera();

private:
	QMutex _mutex;

	bool m_runningFlag;
	bool _stopFlag;

	//
	int m_camFile;

	char* m_camData[CAM_STREAM_FRAMES];
	int m_camDataSize;

	int m_camIndex;

	int m_camWidth;
	int m_camHeight;

	int _camDelay;

signals:
	void signalCamStream(char * camData, unsigned int offset);

public slots:

public:
	inline int getCurrentCamera()
	{
		return m_camIndex;
	}

	inline int getCamWidth() { return m_camWidth; }
	inline int getCamHeight() { return m_camHeight; }

};

#endif // CAMTHREAD_H
