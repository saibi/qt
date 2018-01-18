#include "camthread.h"

#include <QMutexLocker>

#include <iostream>
#include <QColor>

#ifdef __linux__
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#include <linux/fs.h>
#include <linux/kernel.h>
#if defined(__arm__) && defined(__arm_V210__)
#include <linux/videodev.h>
#else
#include <linux/videodev2.h>
#endif
#endif

#define FB_DEVICE 	"/dev/fb0" /* Device Name */

#define CAM_DEVICE 	"/dev/video0" /* Device Name */

#define CAM_TYPE_IMAGE      0
#define CAM_TYPE_LENSMETER  1

#if defined( __arm_V210__)
#define OUTPUT_PIXELFORMAT	V4L2_PIX_FMT_YUV420
#elif defined( __arm_A20__)
#define OUTPUT_PIXELFORMAT	V4L2_PIX_FMT_GREY
#endif

#ifdef __linux__
/* CID extensions */
#define V4L2_CID_ACTIVE_CAMERA		(V4L2_CID_PRIVATE_BASE + 0)
#define V4L2_CID_NR_FRAMES			(V4L2_CID_PRIVATE_BASE + 1)
#define V4L2_CID_RESET				(V4L2_CID_PRIVATE_BASE + 2)
#undef V4L2_CID_TEST_PATTERN
#define V4L2_CID_TEST_PATTERN		(V4L2_CID_PRIVATE_BASE + 3)
#define V4L2_CID_SCALER_BYPASS		(V4L2_CID_PRIVATE_BASE + 4)
#define V4L2_CID_JPEG_INPUT			(V4L2_CID_PRIVATE_BASE + 5)
#define V4L2_CID_OUTPUT_ADDR		(V4L2_CID_PRIVATE_BASE + 10)
#define V4L2_CID_INPUT_ADDR			(V4L2_CID_PRIVATE_BASE + 20)
#define V4L2_CID_INPUT_ADDR_RGB		(V4L2_CID_PRIVATE_BASE + 21)
#define V4L2_CID_INPUT_ADDR_Y		(V4L2_CID_PRIVATE_BASE + 22)
#define V4L2_CID_INPUT_ADDR_CB		(V4L2_CID_PRIVATE_BASE + 23)
#define V4L2_CID_INPUT_ADDR_CBCR	(V4L2_CID_PRIVATE_BASE + 24)
#define V4L2_CID_INPUT_ADDR_CR		(V4L2_CID_PRIVATE_BASE + 25)
#define V4L2_CID_EFFECT_ORIGINAL	(V4L2_CID_PRIVATE_BASE + 30)
#define V4L2_CID_EFFECT_ARBITRARY	(V4L2_CID_PRIVATE_BASE + 31)
#define V4L2_CID_EFFECT_NEGATIVE 	(V4L2_CID_PRIVATE_BASE + 33)
#define V4L2_CID_EFFECT_ARTFREEZE	(V4L2_CID_PRIVATE_BASE + 34)
#define V4L2_CID_EFFECT_EMBOSSING	(V4L2_CID_PRIVATE_BASE + 35)
#define V4L2_CID_EFFECT_SILHOUETTE	(V4L2_CID_PRIVATE_BASE + 36)
#define V4L2_CID_ROTATE_ORIGINAL	(V4L2_CID_PRIVATE_BASE + 40)
#define V4L2_CID_ROTATE_90			(V4L2_CID_PRIVATE_BASE + 41)
#define V4L2_CID_ROTATE_180			(V4L2_CID_PRIVATE_BASE + 42)
#define V4L2_CID_ROTATE_270			(V4L2_CID_PRIVATE_BASE + 43)
#define V4L2_CID_ROTATE_90_HFLIP	(V4L2_CID_PRIVATE_BASE + 44)
#define V4L2_CID_ROTATE_90_VFLIP	(V4L2_CID_PRIVATE_BASE + 45)
#define	V4L2_CID_ZOOM_IN			(V4L2_CID_PRIVATE_BASE + 51)
#define V4L2_CID_ZOOM_OUT			(V4L2_CID_PRIVATE_BASE + 52)

// by zwolf
#define V4L2_CID_CAM_SELECT		V4L2_CID_PRIVATE_BASE+60
#define V4L2_CID_DIRECT_WRITE	V4L2_CID_PRIVATE_BASE+61
#define V4L2_CID_DIRECT_READ	V4L2_CID_PRIVATE_BASE+62

#ifndef LOWORD
#define LOWORD(l) ((quint16)(quint32)(l))
#endif
#ifndef HIWORD
#define HIWORD(l) ((quint16)((((quint32)(l)) >> 16) & 0xFFFF))
#endif
#ifndef MAKELONG
#define MAKELONG(low, high) ((quint32)(((quint16)(low)) | (((quint32)((quint16)(high))) << 16)))
#endif

#endif

#ifndef MAKELONG
#define MAKELONG(low, high) ((quint32)(((quint16)(low) | (((quint32)((quint16)(high))) << 16))))
#endif

#ifdef __arm__
struct fb_var_screeninfo g_fbVar;
#endif


CamThread::CamThread(QObject *parent) :
	QThread(parent),
	_mutex(QMutex::Recursive)
{
	m_runningFlag = false;

	m_camFile = 0;

	for (int i = 0; i < CAM_STREAM_FRAMES; i++)
	{
		m_camData[i] = 0;
	}
	m_camDataSize = 0;

	m_camIndex = NoSelectedCamera;

	m_camWidth = 0;
	m_camHeight = 0;

	_camDelay = 10;

	m_rowVal = 0;
	m_colVal = 0;
}

bool CamThread::startCam(int cam_size)
{
#ifdef __arm__
	QMutexLocker locker(&_mutex);

	if(cam_size != CAM_SIZE_x1_0 && cam_size != CAM_SIZE_x2_0 )
	{
		qDebug("[CamThread::startCam] Error - invalid camear size: %d", cam_size);
		qDebug("[CamThread::startCam] Using default camera size - 480x480.");
		cam_size = CAM_SIZE_x1_0;
	}

	if (m_runningFlag)
		stopCam();

	int mirrorMode = NoMirror;

	m_camIndex = ImageCamera;

	m_camWidth = cam_size;
	m_camHeight = cam_size;

	//
	for (int i = 0; i < CAM_STREAM_FRAMES; i++)
	{
		m_camData[i] = 0;
	}
	m_camDataSize = 0;

	//-------------------------------------------------------------------------
	// camera
	m_camFile = initCam(mirrorMode);

	if (m_camFile <= 0)
	{
		m_camIndex = NoSelectedCamera;
		return false;
	}

	qDebug("[CamThread::startCam] OK - initCam");

	if ( g_ctrl(V4L2_CID_AUDIO_BASS, m_rowVal) == 0 && g_ctrl(V4L2_CID_AUDIO_TREBLE, m_colVal) == 0)
	{
		m_startRowCol.setX(m_colVal);
		m_startRowCol.setY(m_rowVal);
		qDebug("[CamThread::startCam] OK - row,col");
	}

	locker.unlock();

	// call run();
	start(QThread::LowPriority);

	msleep(10);
#else
	Q_UNUSED(cam_size);
#endif
	return true;
}

void CamThread::stopCam()
{
#ifdef __arm__
	QMutexLocker locker(&_mutex);

	if (m_runningFlag == false)
		return;

	_stopFlag = true;

	wait();

	//-------------------------------------------------------------------------
	{
		int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (::ioctl(m_camFile, VIDIOC_STREAMOFF, &type) == -1)
		{
			qDebug("[CamThread::stopCam] Error - ioctl(camFile, VIDIOC_STREAMOFF, &type)");
		}
	}

	for (int i = 0; i < CAM_STREAM_FRAMES; i++)
	{
		if (m_camData[i] && m_camDataSize)
		{
			::munmap(m_camData[i], m_camDataSize);
			msleep(_camDelay);
		}
		m_camData[i] = 0;
	}
	m_camDataSize = 0;

	m_camIndex = NoSelectedCamera;
#endif
}


void CamThread::run()
{
#ifdef __arm__
	if (m_runningFlag)
		return;

	m_runningFlag = true;

	_stopFlag = false;

	switch (m_camIndex)
	{
	case ImageCamera:
		runNormalCamera();
		break;

	default:
		qDebug("[CamThread::run] Error - wrong camera index: %d", m_camIndex);
	}

	m_runningFlag = false;
#endif
}

int CamThread::initCam(int mirrorMode)
{			
#ifdef __arm__
	int camFile = 0;

	if (m_camFile)
	{
		camFile = m_camFile;
	}
	else
	{
#ifdef __arm_A20__
		camFile = ::open(CAM_DEVICE, O_RDWR | O_NONBLOCK, 0);
#else
		camFile = ::open(CAM_DEVICE, O_RDONLY);
#endif
		if (camFile <= 0)
		{
			qDebug("[CamThread::initCam] Error - open(CAM_DEVICE, O_RDONLY)");
			return 0;
		}
	}

	msleep(_camDelay);

#ifdef __arm__
	//-------------------------------------------------------------------------
	{
		int input = 0;

		if (::ioctl(camFile, VIDIOC_S_INPUT, &input) < 0)
		{
			qDebug("[CamThread::initCam] Error - ioctl(camFile, VIDIOC_S_INPUT, &input), errno - %d", errno);
			goto INIT_CAM_ERROR;
		}
	}

	msleep(_camDelay);
#endif

	//-------------------------------------------------------------------------
	{
		struct v4l2_format vfmt;

		vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  // V4L2_CAP_VIDEO_OVERLAY

		vfmt.fmt.pix.width = m_camWidth;
		vfmt.fmt.pix.height = m_camHeight;

		vfmt.fmt.pix.pixelformat = OUTPUT_PIXELFORMAT;

		vfmt.fmt.pix.priv = 0;
#ifdef __arm_A20__
		vfmt.fmt.pix.field = V4L2_FIELD_NONE;

		qDebug("DBG pix.width = %d, pix.height = %d", m_camWidth, m_camHeight);
#endif
		if (::ioctl(camFile, VIDIOC_S_FMT, &vfmt) < 0)
		{
			qDebug("[CamThread::initCam] Error - VIDIOC_S_FMT, errno - %d", errno);
			goto INIT_CAM_ERROR;
		}
	}

	msleep(_camDelay);

	// Allocate buffer for captured image
	//-------------------------------------------------------------------------
	{
		struct v4l2_requestbuffers req;
		unsigned int loop;
		int qcnt;

		qcnt = 4;

		req.count = qcnt;
		req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory = V4L2_MEMORY_MMAP;

		if (::ioctl(camFile, VIDIOC_REQBUFS, &req) < 0)
		{
			qDebug("[CamThread::initCam] Error - VIDIOC_REQBUFS, errno - %d", errno);
			goto INIT_CAM_ERROR;
		}

		for (loop = 0; loop < req.count; loop++)
		{
			struct v4l2_buffer buf;
			int i = loop;

			buf.index = loop;
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			buf.length = 0;
			buf.m.offset = 0;

			if (::ioctl(camFile, VIDIOC_QUERYBUF, &buf) < 0)
			{
				qDebug("[CamThread::initCam] Error - VIDIOC_QUERYBUF, errno=%d", errno);
				qDebug("EBADF(%d),EINTR(%d),EINVAL(%d),EIO(%d),ENOTTY(%d),ENXIO(%d),ENODEV(%d)", EBADF,EINTR,EINVAL,EIO,ENOTTY,ENXIO,ENODEV);
				goto INIT_CAM_ERROR;
			}

			qDebug("[CamThread::initCam] VIDIOC_QUERYBUF - index: %d, length: %d, offset: %d", buf.index, buf.length, buf.m.offset);

			int prot = PROT_READ;
#ifdef __arm_A20__
			prot |= PROT_WRITE;
#endif
			m_camData[i] = (char *)mmap(0, buf.length, prot, MAP_SHARED, camFile, buf.m.offset);
			m_camDataSize = buf.length;

			if (m_camData[i] == MAP_FAILED)
			{
				qDebug("[CamThread::initCam] Error - mmap (%d)", errno);
				goto INIT_CAM_ERROR;
			}
#ifdef __arm_A20__
			if ( -1 == ::ioctl(camFile, VIDIOC_QBUF, &buf))
			{
				qDebug("[CamThread::initCam] Error - VIDIOC_QBUF, errno - %d", errno);
			}
#endif
			qDebug("m_camData[%d] = %p, m_camDataSize = %d", i, m_camData[i], m_camDataSize);
		}
	}

	msleep(_camDelay);

#ifdef __arm_V210__
	//-------------------------------------------------------------------------
	{
		struct v4l2_control ctrl;

		ctrl.id = V4L2_CID_VFLIP;
		ctrl.value = (mirrorMode == ColumnMirror) ? 1 : 0;

		qDebug("[CamThread::initCam] V4L2_CID_VFLIP (%d)", ctrl.value);

		if (::ioctl(camFile, VIDIOC_S_CTRL, &ctrl) == -1)
		{
			qDebug("[CamThread::initCam] Error - ioctl(camFile, VIDIOC_S_CTRL, V4L2_CID_VFLIP), errno - %d", errno);
			goto INIT_CAM_ERROR;
		}

		msleep(_camDelay);

		ctrl.id = V4L2_CID_HFLIP;
		ctrl.value = (mirrorMode == RowMirror) ? 1 : 0;

		qDebug("[CamThread::initCam] V4L2_CID_HFLIP (%d)", ctrl.value);

		if (::ioctl(camFile, VIDIOC_S_CTRL, &ctrl) == -1)
		{
			qDebug("[CamThread::initCam] Error - ioctl(camFile, VIDIOC_S_CTRL, V4L2_CID_HFLIP), errno - %d", errno);
			goto INIT_CAM_ERROR;
		}
	}
#endif

	msleep(_camDelay);

	//-------------------------------------------------------------------------
	{
		int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (::ioctl(camFile, VIDIOC_STREAMON, &type) == -1)
		{
			qDebug("[CamThread::initCam] Error - ioctl(camFile, VIDIOC_STREAMON, &type), errno - %d", errno);
			goto INIT_CAM_ERROR;
		}
	}

//	msleep(_camDelay);

	//std::cout << "[CamThread::initCam] Step - " << 13 << std::endl << std::flush;

	return camFile;

INIT_CAM_ERROR:
	if(camFile != 0) ::close(camFile);
	return 0;
#else
	Q_UNUSED(mirrorMode);
	return false;
#endif
}

void CamThread::runNormalCamera()
{
#ifdef __arm__
	struct v4l2_buffer camBuffer;

	char* camData;

	int ret;
	fd_set rdSet;
	struct timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	qDebug("[CamThread::runNormalCamera] STARTED");

	while (!_stopFlag)
	{
		memset(&camBuffer, 0, sizeof(camBuffer));

		camBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		camBuffer.memory = V4L2_MEMORY_MMAP;

		FD_ZERO(&rdSet);
		FD_SET(m_camFile, &rdSet);

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		ret = select(m_camFile + 1, &rdSet, NULL, NULL, &timeout);

		if (ret == -1)
		{
			qDebug("[CamThread::runNormalCamera] Error - select");
			continue;
		}
		else if (ret == 0)
		{
			qDebug("[CamThread::runNormalCamera] Error - timeout");
			continue;
		}
		else if (FD_ISSET(m_camFile, &rdSet))
		{	
			if ( -1 == ::ioctl(m_camFile, VIDIOC_DQBUF, &camBuffer) )
			{
				qDebug("[CamThread::runNormalCamera] Error - VIDIOC_DQBUF (%d)", errno);
				continue;
			}

#ifdef __arm_A20__
			if ( -1 == ::ioctl(m_camFile, VIDIOC_QBUF, &camBuffer))
			{
				qDebug("[CamThread::runNormalCamera] Error - VIDIOC_QBUF, errno - %d", errno);
			}
#endif
		}
		camData = m_camData[camBuffer.index];
		emit signalCamStream(camData, camBuffer.m.offset);
	}
	qDebug("[CamThread::runNormalCamera] STOPED");
#endif
}


int CamThread::getCurrentCameraSize()
{
	return m_camWidth;
}

bool CamThread::isRunning()
{
	QMutexLocker locker(&_mutex);

	return m_runningFlag;
}

int CamThread::s_ctrl(int id, int value)
{
	struct v4l2_control ctl;

	ctl.id = id;
	ctl.value = value;

	return ::ioctl(m_camFile, VIDIOC_S_CTRL, &ctl);
}

int CamThread::g_ctrl(int id, int &value)
{
	struct v4l2_control ctl;

	ctl.id = id;

	int ret = ::ioctl(m_camFile, VIDIOC_G_CTRL, &ctl);
	if ( ret == 0 )
		value = ctl.value;

	return ret;
}

void CamThread::adjRow(bool up)
{
	if ( up )
	{
		if ( m_rowVal < 512)
		{
			++m_rowVal;
			if ( s_ctrl(V4L2_CID_AUDIO_BASS, m_rowVal) == 0 )
				qDebug("m_rowVal = %d", m_rowVal);
		}
	}
	else
	{
		if ( m_rowVal > 0 )
		{
			--m_rowVal;
			if ( s_ctrl(V4L2_CID_AUDIO_BASS, m_rowVal) == 0 )
				qDebug("m_rowVal = %d", m_rowVal);
		}
	}
}

void CamThread::adjCol(bool up)
{

	if ( up )
	{
		if ( m_colVal < 639)
		{
			++m_colVal;
			if ( s_ctrl(V4L2_CID_AUDIO_TREBLE, m_colVal) == 0 )
				qDebug("m_colVal = %d", m_colVal);
		}
	}
	else
	{
		if ( m_colVal > 0 )
		{
			--m_colVal;
			if ( s_ctrl(V4L2_CID_AUDIO_TREBLE, m_colVal) == 0 )
				qDebug("m_colVal = %d", m_colVal);
		}
	}
}

void CamThread::resetRowCol()
{
	m_rowVal = m_startRowCol.y();
	m_colVal = m_startRowCol.x();

	if ( s_ctrl(V4L2_CID_AUDIO_BASS, m_rowVal) == 0 )
		qDebug("m_rowVal = %d", m_rowVal);

	if ( s_ctrl(V4L2_CID_AUDIO_TREBLE, m_colVal) == 0 )
		qDebug("m_colVal = %d", m_colVal);
}
