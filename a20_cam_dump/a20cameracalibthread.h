#ifndef A20CAMERACALIBTHREAD_H
#define A20CAMERACALIBTHREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QTime>

#ifdef __arm_A20__
#include <linux/videodev2.h>
#endif

struct CameraDisplayData
{
#ifdef __arm_A20__
	struct v4l2_buffer camBuffer;
#endif
	char * camAddr;
	bool dispUpdate;
};

#ifdef PUFF_DUMP_IMAGE
struct DumpItem
{
	int no;
	QTime time;
	QImage img;
};
#endif

class A20CameraCalibThread : public QThread
{
	Q_OBJECT

public:
	explicit A20CameraCalibThread(QObject * parent = 0);

	void startThread(int camWidth, int camHeight, QVector< QVector<QPointF> >& calibrationPosition);
	void stop();

	bool calibNDisp(struct CameraDisplayData *p);
	char * getLastDispBuf();
	int getDispCount() { return m_dispCount; }
	bool getDoneList(QList <struct CameraDisplayData> & list);

	void setCompensationOn(bool bCompensation) { m_bCompensation = bCompensation; }
	void setCalibrationData(QVector< QVector<QPointF> >& calibrationPosition);

#ifdef PUFF_DUMP_IMAGE
	bool dispNSave(char * camBuf, unsigned int offset);
#endif

protected:
	void run();
	void calibrateImage(char *distortedImage, QImage& tempImage);

private:
	QMutex m_mutex;

	bool m_running;
	bool m_stopFlag;

	int m_reqCount;
	int m_dispCount;

	int m_camWidth;
	int m_camHeight;

	QImage m_tmpImg;
	QVector<QRgb> m_yuv_rgb_conv_color_tab;
	QVector< QVector<QPointF> > m_calibrationPosition;

	bool m_bCompensation;

	QList <struct CameraDisplayData> m_waitList, m_dispList, m_doneList;


#ifdef PUFF_DUMP_IMAGE
	QList <struct DumpItem> m_imgList;
#endif
};

#endif // A20CAMERACALIBTHREAD_H
