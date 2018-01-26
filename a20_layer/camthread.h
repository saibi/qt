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
	static CamThread& instance()
	{
		static CamThread cam_thread;
		return cam_thread;
	}


	enum CameraSizeIndex
	{
		CAM_SIZE_480 =	480,	// 480 * 480
		CAM_SIZE_640 = 640,	// 640 * 480
		CAM_SIZE_960 = 960,	// 960 * 960
		CAM_SIZE_1280 =	1280, // 1280 * 1024
	};

	bool startCam(int cam_size = CAM_SIZE_480);
	void stopCam();

	bool isRunning();

	bool adjRow(bool up, int step = 1);
	bool adjCol(bool up, int step = 1);
	bool resetRowCol();
	bool adjGain(bool up, int step = 1);
	bool adjExp(bool up, int step = 1);
	bool vflip(bool on);

	void closeCam();

protected:
	void run();

	int initCam();
	void runCamera();

	bool s_ctrl(int id, int value);
	bool g_ctrl(int id, int & value);
	bool adj_v4l2_cid(int v4l2_id, int & var, bool up, int min, int max, const char *pr_name, int step = 1);

private:

	enum InternalConstants
	{
		CAM_STREAM_FRAMES = 4, /* Number of streaming buffer */
	};

	QMutex _mutex;

	bool m_runningFlag;
	bool _stopFlag;

	//
	int m_fdCam;

	char* m_camData[CAM_STREAM_FRAMES];
	int m_camDataSize;
	int m_camWidth;
	int m_camHeight;
	int _camDelay;

	struct CamCtrlValues
	{
		int sRow;
		int sCol;
		int row;
		int col;
		int gain;
		int exp;
	} m_ctrl;

signals:
	void signalCamStream(char * camData, unsigned int offset);

public slots:

public:

	inline int getCamWidth() { return m_camWidth; }
	inline int getCamHeight() { return m_camHeight; }

	inline int getGain() { return m_ctrl.gain; }
	inline int getExp() { return m_ctrl.exp; }
	inline int getRow() { return m_ctrl.row; }
	inline int getCol() { return m_ctrl.col; }

};

#endif // CAMTHREAD_H
