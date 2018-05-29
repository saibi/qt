#ifndef A20CAMTHREAD_H
#define A20CAMTHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>

#ifdef PUFF_SEP_CAM_CALIB_THREAD
#include "a20cameracalibthread.h"
#endif

// define A20_CAM_THREAD_USE_DISP in the qmake.pro to display camera image in the camthread
// or define A20_CAM_THREAD_EMIT_SIGNAL in the qmake.pro to emit camera stream signal in the camthread

// for compatibility
#define CAM_SIZE_x1_0		480
#define CAM_SIZE_x2_0		960
#define CAM_SIZE_x1_5		720 // not supported
#define CAM_SIZE_x1_3		624 // not supported

#ifdef WIN32
#define V4L2_CID_GAIN	0
#define V4L2_CID_HFLIP	0
#define V4L2_CID_VFLIP	0
#define V4L2_CID_AUDIO_BASS	0
#define V4L2_CID_AUDIO_TREBLE	0
#define V4L2_CID_EXPOSURE	0
#define V4L2_CID_VFLIP	0
#define V4L2_CID_VFLIP	0
#define V4L2_CID_VFLIP	0
#endif

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

	// A20 camera
	enum CameraSizeIndex
	{
		// working on kernel 3.4.104e

		SIZE_120s = 120, // 120 x 120 *
		SIZE_128s = 128, // 128 x 128 *
		SIZE_144s = 144, // 144 x 144 *
		SIZE_176s = 176, // 176 x 176 *
		SIZE_200s = 200, // 200 x 200 *
		SIZE_240s = 240, // 240 x 240 *
		SIZE_256s = 256, // 256 x 256 *
		SIZE_288s = 288, // 288 x 288 *
		SIZE_320s = 320, // 320 x 320 *
		SIZE_352s = 352, // 352 x 352 *
		SIZE_360s = 360, // 360 x 360 *
		SIZE_384s = 384, // 384 x 384 *
		SIZE_400s = 400, // 400 x 400 *
		SIZE_432s = 432, // 432 x 432 *
		SIZE_480s = 480, // 480 x 480 *
		SIZE_512 = 512, // 512 x 512
		SIZE_600 = 600, // 600 x 600
		SIZE_624 = 624, // 624 x 624
		SIZE_640 = 640, // 640 x 640
		SIZE_720 = 720, // 720 x 720
		SIZE_768 = 768, // 768 x 768
		SIZE_800 = 800, // 800 x 800
		SIZE_816 = 816, // 816 x 816
		SIZE_960 = 960,	// 960 x 960
		SIZE_1024 = 1024, // 1024 x 1024
		SIZE_1280 =	1280, // 1280 x 1024
	};

	bool startCameraThread(int cam_size = SIZE_480s, const QPoint & centerDiff = QPoint(0, 0));
	void stopCameraThread();
	void closeCameraDevice();

	bool adjRow(bool up, int step = 1);
	bool adjCol(bool up, int step = 1);
	bool adjGain(bool up, int step = 1);
	bool adjExp(bool up, int step = 1);

	bool setGain(int value);
	bool setExp(int value);

	bool vflip(bool on); // not working
	bool hflip(bool on);

	bool setRow(int value);
	bool setCol(int value);

	enum CtrlValueMax
	{
		MAX_ROW = 511,
		MAX_COL = 639,
		MAX_GAIN = 127,
		MAX_EXP = 127,

		CAM_LIMIT_HORZ = 180,
		CAM_LIMIT_VERT = 44,
	};

	bool size2wh(int camSize, int *w, int *h);

	// for compatibility
	bool isRunning();

	bool startCam(int cam_idx = ImageCamera, const QPoint & centerDiff = QPoint(0, 0), int cam_size = CAM_SIZE_x1_0, bool clear_FB = true);
	void stopCam();
	void setTopLeftOnFB(const QPoint & topleft_pos);
	bool moveCam(int camPosX, int camPosY);

	enum CameraIndex
	{
		ImageCamera = 0,
		LensmeterCamera = 1, // not supported

		NoSelectedCamera = 5, // not supported
	};

	bool captureCam(QImage *image);
	bool captureCam(QImage *image, int cam_index, const QPoint & cam_center_diff, int cam_size);

	void setCalibrationData(QVector< QVector<QPointF> >& calibrationPosition);
	void setCompensationOn(bool bCompensation);

	static QString screenShotDir;
	static bool screenShotDateTime;


	bool test_stream(bool flag);
	void test_960scaling(bool flag) { m_960scaling = flag; };

protected:
	void run();

	int initCameraDevice();
	void camLoop();

	bool s_ctrl(int id, int value);
	bool g_ctrl(int id, int & value);
	bool adj_v4l2_cid(int v4l2_id, int & var, bool up, int min, int max, const char *pr_name, int step = 1);
	bool setRowCol(int row, int col);

	bool set_v4l2_cid(int v4l2_id, int & var, int min, int max, const char *pr_name, int value);

	bool save();
	void restore();

	void setDispStreamOn(bool flag);

	QImage combineCameraAndFrameBuffer(const QImage & imgCam, const QImage & imgFb, bool scaling = false, int scalingSrcSize = 0); // imgCam indexed-8, imgFb RGB32 ==> RGB16

private:

	enum InternalConstants
	{
		CAM_STREAM_BUFFERS = 8, /* Number of streaming buffer */
	};

	QMutex _mutex;

	bool m_runningFlag;
	bool _stopFlag;
	int m_fdCam;
	char* m_camData[CAM_STREAM_BUFFERS];
	int m_camDataSize;
	int m_camWidth;
	int m_camHeight;
	int m_camSize; // set in the startCameraThread()
	bool m_streamOn;
	int _camDelay;
	char * m_currentCamStreamBuffer;

	int m_startCamSize; // set in the startCam()

	QMutex _captureMutex;
	QImage* _captureImage;

	struct CamCtrlValues
	{
		int sRow; // center raw
		int sCol; // center col
		int row;
		int col;
		int gain;
		int exp;
	} m_ctrl;

	struct saveData
	{
		bool saved;
		int width;
		bool streamOn;
		QPoint centerDiff;
	} m_saved;

	QPoint m_topLeftPos;

	// convert
	QVector<QRgb> m_yuv_rgb_conv_color_tab;


	// for disp engine
	bool m_dispCam;
	QPoint m_dispSize;
	bool m_showCamHideFB;
	bool m_dispScaling;
	int m_scalingSrcSize;
	bool m_960scaling;

	QMutex _compensationMutex;
	bool m_bCompensation;
	
	QVector< QVector<QPointF> > m_calibrationPosition;
#ifdef PUFF_CAM_THREAD_CALIB
	void calibrateImage(char* distortedImage, QImage& tempImage);
#endif

	// screenshot
	QImage * m_imageScreenShot;
	int m_screenShotType;
	enum ScreenShotTypeIdx
	{
		SC_NONE = 0,
		SC_NORMAL = 1, // SIGHUP
		SC_ALL = 10, // SIGUSR1
		SC_960_ALL = 12, // SIGUSR2
	};


#ifdef PUFF_SEP_CAM_CALIB_THREAD
	A20CameraCalibThread m_calibThread;
#endif

signals:
	void signalCaptureComplete(QImage *image);

private slots:
	void slotCaptureComplete(QImage *image);

public:

	// A20 camera
	inline int getCamWidth() { return m_camWidth; }
	inline int getCamHeight() { return m_camHeight; }

	inline int getGain() { return m_ctrl.gain; }
	inline int getExp() { return m_ctrl.exp; }
	inline int getRow() { return m_ctrl.row; }
	inline int getCol() { return m_ctrl.col; }

	QPoint getCenterDiff() { return QPoint(m_ctrl.sCol - m_ctrl.col, m_ctrl.row - m_ctrl.sRow); };

	inline void setDispWindowSize(int w, int h) { m_dispSize.setX(w); m_dispSize.setY(h); };

	// for compatibility
	inline int getCurrentCamera() { return ImageCamera; }
	void setStreamOn(bool flag);

	inline void setColorTabToGrayColorize() {} // not supported
	inline void setColorTabToNormal() {} // not supported

	inline int getCurrentCameraSize() { return m_startCamSize; }

signals:
	void signalCamStream(char * camData, unsigned int offset);
	void imageCapture(QImage *image);

public slots:
	void slotUnixSignal(int signal);
};

#endif // A20CAMTHREAD_H
