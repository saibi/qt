#ifndef CAMTHREAD_H
#define CAMTHREAD_H

#include <QThread>

#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <QPoint>

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

	bool startCam(int cam_size = CAM_SIZE_480);
	void stopCam();

	bool isRunning();


	void adjRow(bool up);
	void adjCol(bool up);
	void resetRowCol();

	enum CameraSizeIndex
	{
		CAM_SIZE_480 =	480,	// 480 * 480
		CAM_SIZE_640 = 640,	// 640 * 480
		CAM_SIZE_960 = 960,	// 960 * 960
		CAM_SIZE_1280 =	1280, // 1280 * 1024
	};

protected:
	void run();

	int initCam(int mirrorMode = NoMirror);

	void runNormalCamera();

	int s_ctrl(int id, int value);
	int g_ctrl(int id, int & value);

private:

	enum InternalConstants
	{
		CAM_STREAM_FRAMES = 4, /* Number of streaming buffer */
	};


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

	int m_rowVal;
	int m_colVal;
	QPoint m_startRowCol;
signals:
	void signalCamStream(char * camData, unsigned int offset);

public slots:

public:

	inline int getCamWidth() { return m_camWidth; }
	inline int getCamHeight() { return m_camHeight; }

};

#endif // CAMTHREAD_H
