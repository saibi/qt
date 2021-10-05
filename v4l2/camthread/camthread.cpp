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
#include <linux/videodev2.h>
#endif

#define CAM_DEVICE 	"/dev/video0" /* Device Name */


CamThread::CamThread(QObject *parent) :
	QThread(parent),
	_mutex(QMutex::Recursive)
{
	m_runningFlag = false;

	m_fdCam = 0;

	for (int i = 0; i < CAM_STREAM_FRAMES; i++)
	{
		m_camData[i] = 0;
	}
	m_camDataSize = 0;

	m_camWidth = 0;
	m_camHeight = 0;

	_camDelay = 10;

	::memset(&m_ctrl, 0, sizeof(m_ctrl));
}

bool CamThread::startCam(int cam_size)
{
	QMutexLocker locker(&_mutex);

	if (m_runningFlag)
		stopCam();

	switch (cam_size )
	{

	case CAM_SIZE_640:
	default:
		m_camWidth = 640;
		m_camHeight = 480;
		break;
	}

	//
	for (int i = 0; i < CAM_STREAM_FRAMES; i++)
	{
		m_camData[i] = 0;
	}
	m_camDataSize = 0;

	//-------------------------------------------------------------------------
	// camera
	m_fdCam = initCam();
	if (m_fdCam <= 0)
	{
		return false;
	}

	qDebug("[CamThread::startCam] OK - initCam");

	if ( g_ctrl(V4L2_CID_AUDIO_BASS, m_ctrl.row) && g_ctrl(V4L2_CID_AUDIO_TREBLE, m_ctrl.col) && g_ctrl(V4L2_CID_GAIN, m_ctrl.gain) && g_ctrl(V4L2_CID_EXPOSURE, m_ctrl.exp) )
	{
		m_ctrl.sRow = m_ctrl.row;
		m_ctrl.sCol = m_ctrl.col;

		qDebug("[%s] row = %d, col = %d, gain = %d, exp = %d", Q_FUNC_INFO, m_ctrl.row, m_ctrl.col, m_ctrl.gain, m_ctrl.exp);
	}

	locker.unlock();

	// call run();
	start(QThread::LowPriority);

	msleep(10);
	return true;
}

void CamThread::stopCam()
{
	QMutexLocker locker(&_mutex);

	if (m_runningFlag == false)
		return;

	_stopFlag = true;

	wait();

	{
		int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (::ioctl(m_fdCam, VIDIOC_STREAMOFF, &type) == -1)
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
}


void CamThread::run()
{
	if (m_runningFlag)
		return;

	m_runningFlag = true;

	_stopFlag = false;

	runCamera();

	m_runningFlag = false;
}

int CamThread::initCam()
{			
	int camFile = 0;

	if (m_fdCam)
	{
		camFile = m_fdCam;
	}
	else
	{
		camFile = ::open(CAM_DEVICE, O_RDWR | O_NONBLOCK, 0);
		if (camFile <= 0)
		{
			qDebug("[CamThread::initCam] Error - open(CAM_DEVICE, O_RDONLY)");
			return 0;
		}
	}

	msleep(_camDelay);

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

	//-------------------------------------------------------------------------
	{
		struct v4l2_format vfmt;

		vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  // V4L2_CAP_VIDEO_OVERLAY

		vfmt.fmt.pix.width = m_camWidth;
		vfmt.fmt.pix.height = m_camHeight;

		vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV422P; // V4L2_PIX_FMT_GREY;

		vfmt.fmt.pix.priv = 0;
		vfmt.fmt.pix.field = V4L2_FIELD_NONE;

		qDebug("DBG pix.width = %d, pix.height = %d", m_camWidth, m_camHeight);
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

		req.count = CAM_STREAM_FRAMES;
		req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory = V4L2_MEMORY_MMAP;

		if (::ioctl(camFile, VIDIOC_REQBUFS, &req) < 0)
		{
			qDebug("[CamThread::initCam] Error - VIDIOC_REQBUFS, errno - %d", errno);
			goto INIT_CAM_ERROR;
		}

		for (unsigned int loop = 0; loop < req.count; ++loop)
		{
			struct v4l2_buffer buf;

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

			m_camData[loop] = (char *)mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camFile, buf.m.offset);
			m_camDataSize = buf.length;

			if (m_camData[loop] == MAP_FAILED)
			{
				qDebug("[CamThread::initCam] Error - mmap (%d)", errno);
				goto INIT_CAM_ERROR;
			}

			if ( -1 == ::ioctl(camFile, VIDIOC_QBUF, &buf))
			{
				qDebug("[CamThread::initCam] Error - VIDIOC_QBUF, errno - %d", errno);
			}
			qDebug("m_camData[%d] = %p, m_camDataSize = %d", loop, m_camData[loop], m_camDataSize);
		}
	}

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

	return camFile;

INIT_CAM_ERROR:
	if(camFile != 0) ::close(camFile);
	return 0;
}

void CamThread::runCamera()
{
	struct v4l2_buffer camBuffer;

	char* camData;

	int ret;
	fd_set rdSet;
	struct timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	qDebug("[%s] start", Q_FUNC_INFO);

	while (!_stopFlag)
	{
		memset(&camBuffer, 0, sizeof(camBuffer));

		camBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		camBuffer.memory = V4L2_MEMORY_MMAP;

		FD_ZERO(&rdSet);
		FD_SET(m_fdCam, &rdSet);

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		ret = select(m_fdCam + 1, &rdSet, NULL, NULL, &timeout);

		if (ret == -1)
		{
			qDebug("[%s] select error %d", Q_FUNC_INFO, errno);
			continue;
		}
		else if (ret == 0)
		{
			qDebug("[%s] select timeout", Q_FUNC_INFO);
			continue;
		}
		else if (FD_ISSET(m_fdCam, &rdSet))
		{	
			if ( -1 == ::ioctl(m_fdCam, VIDIOC_DQBUF, &camBuffer) )
			{
				qDebug("[%s] VIDIOC_DQBUF error %d", Q_FUNC_INFO, errno);
				continue;
			}

			if ( -1 == ::ioctl(m_fdCam, VIDIOC_QBUF, &camBuffer))
			{
				qDebug("[%s] VIDIOC_QBUF error %d", Q_FUNC_INFO, errno);
				continue;
			}
		}
		camData = m_camData[camBuffer.index];
		emit signalCamStream(camData, camBuffer.m.offset);
	}
	qDebug("[%s] end", Q_FUNC_INFO);
}

bool CamThread::isRunning()
{
	QMutexLocker locker(&_mutex);

	return m_runningFlag;
}

bool CamThread::s_ctrl(int id, int value)
{
	struct v4l2_control ctl;

	ctl.id = id;
	ctl.value = value;

	if ( ::ioctl(m_fdCam, VIDIOC_S_CTRL, &ctl) == 0 )
		return true;
	else
		return false;
}

bool CamThread::g_ctrl(int id, int &value)
{
	struct v4l2_control ctl;

	ctl.id = id;

	if ( ::ioctl(m_fdCam, VIDIOC_G_CTRL, &ctl) == 0 )
	{
		value = ctl.value;
		return true;
	}
	else
		return false;
}

bool CamThread::adjRow(bool up, int step)
{
	return adj_v4l2_cid(V4L2_CID_AUDIO_BASS, m_ctrl.row, up, 0, 511, "row", step);
}

bool CamThread::adjCol(bool up, int step)
{
	return adj_v4l2_cid(V4L2_CID_AUDIO_TREBLE, m_ctrl.col, up, 0, 639, "col", step);
}

bool CamThread::resetRowCol()
{
	if ( s_ctrl(V4L2_CID_AUDIO_BASS, m_ctrl.sRow) && s_ctrl(V4L2_CID_AUDIO_TREBLE, m_ctrl.sCol) )
	{
		m_ctrl.row = m_ctrl.sRow;
		m_ctrl.col = m_ctrl.sCol;
		qDebug("row = %d, col = %d", m_ctrl.row, m_ctrl.col);
		return true;
	}
	return false;
}

bool CamThread::adjGain(bool up, int step)
{
	return adj_v4l2_cid(V4L2_CID_GAIN, m_ctrl.gain, up, 0, 127, "gain", step);
}

bool CamThread::adjExp(bool up, int step)
{
	return adj_v4l2_cid(V4L2_CID_EXPOSURE, m_ctrl.exp, up, 0, 127, "exp", step);
}

bool CamThread::adj_v4l2_cid(int v4l2_id, int &var, bool up, int min, int max, const char *pr_name, int step)
{
	if ( up )
	{
		if ( var < max)
		{
			var += step;
			if ( s_ctrl(v4l2_id, var) )
			{
				qDebug("%s = %d", pr_name, var);
				return true;
			}
		}
	}
	else
	{
		if ( var > min )
		{
			var -= step;
			if ( s_ctrl(v4l2_id, var) )
			{
				qDebug("%s = %d", pr_name, var);
				return true;
			}
		}
	}
	return false;
}

void CamThread::closeCam()
{
	stopCam();

	if ( m_fdCam > 0 )
		::close(m_fdCam);

	m_fdCam = 0;
}

bool CamThread::vflip(bool on)
{
	if ( s_ctrl(V4L2_CID_VFLIP, on ? 1 : 0) )
	{
		qDebug("[%s] s_ctrl vflip %d ok", Q_FUNC_INFO, on);
		return true;
	}
	else
	{
		qDebug("[%s] s_ctrl vflip %d err", Q_FUNC_INFO, on);
		return false;
	}
}
