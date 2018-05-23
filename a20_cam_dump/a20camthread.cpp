#include "a20camthread.h"

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

#include "a20disp.h"
#include <QDateTime>


#define CAM_DEVICE 	"/dev/video0"

QString CamThread::screenShotDir = "/tmp";
bool CamThread::screenShotDateTime = false;

CamThread::CamThread(QObject *parent) :
	QThread(parent),
	_mutex(QMutex::Recursive)
{
	m_runningFlag = false;

	m_fdCam = 0;

	for (int i = 0; i < CAM_STREAM_BUFFERS ; i++)
	{
		m_camData[i] = 0;
	}
	m_camDataSize = 0;
	m_currentCamStreamBuffer = 0;

	m_camWidth = 0;
	m_camHeight = 0;
	m_camSize = 0;
	_camDelay = 10;

	::memset(&m_ctrl, 0, sizeof(m_ctrl));

	m_streamOn = true;
	m_dispCam = false;
	m_dispSize = QPoint(480, 480);
	m_dispScaling = false;

	m_saved.saved = false;
	m_saved.width = 0;
	m_saved.streamOn = false;

	_captureImage = 0;

	for(int i = 0 ; i < 256 ; ++i)
		m_yuv_rgb_conv_color_tab.append(qRgb(i,i,i));

	m_bCompensation = true;

	m_showCamHideFB = false;

	m_imageScreenShot = 0;
	m_screenShotType = SC_NONE;
	m_startCamSize = 0;

	connect(this, SIGNAL(signalCaptureComplete(QImage*)), this, SLOT(slotCaptureComplete(QImage*)));
}

bool CamThread::size2wh(int camSize, int *w, int *h)
{
	switch (camSize )
	{
	case SIZE_120s:
	case SIZE_128s:
	case SIZE_144s:
	case SIZE_176s:
	case SIZE_200s:
	case SIZE_240s:
	case SIZE_256s:
	case SIZE_288s:
	case SIZE_320s:
	case SIZE_352s:
	case SIZE_360s:
	case SIZE_384s:
	case SIZE_400s:
	case SIZE_432s:
	case SIZE_480s:
	case SIZE_512:
	case SIZE_600:
	case SIZE_624:
	case SIZE_640:
	case SIZE_720:
	case SIZE_768:
	case SIZE_800:
	case SIZE_960:
	case SIZE_1024:
		if ( w )
			*w = camSize;
		if ( h )
			*h = camSize;
		break;

	case SIZE_1280:
		if ( w )
			*w = 1280;
		if ( h )
			*h = 1024;
		break;

	default:
		return false;
	}
	return true;
}

bool CamThread::startCameraThread(int cam_size, const QPoint & centerDiff)
{
#ifdef __arm_A20__
	QMutexLocker locker(&_mutex);

	if (m_runningFlag)
		stopCameraThread();

	if ( size2wh(cam_size, &m_camWidth, &m_camHeight) == false )
	{
		qDebug("[%s] invalid cam_size %d", Q_FUNC_INFO, cam_size);
		return false;
	}

	m_camSize = cam_size;

	//
	for (int i = 0; i < CAM_STREAM_BUFFERS ; i++)
	{
		m_camData[i] = 0;
	}
	m_camDataSize = 0;

	//-------------------------------------------------------------------------
	// camera
	m_fdCam = initCameraDevice();
	if (m_fdCam <= 0)
	{
		return false;
	}

	qDebug("[%s] OK - initCameraDevice", Q_FUNC_INFO);

	if ( g_ctrl(V4L2_CID_AUDIO_BASS, m_ctrl.row) && g_ctrl(V4L2_CID_AUDIO_TREBLE, m_ctrl.col) && g_ctrl(V4L2_CID_GAIN, m_ctrl.gain) && g_ctrl(V4L2_CID_EXPOSURE, m_ctrl.exp) )
	{
		m_ctrl.sRow = m_ctrl.row; // row center = 44
		m_ctrl.sCol = m_ctrl.col; // col center = 180

		qDebug("[%s] row = %d, col = %d, gain = %d, exp = %d", Q_FUNC_INFO, m_ctrl.row, m_ctrl.col, m_ctrl.gain, m_ctrl.exp);
	}

	if ( centerDiff.x() != 0 || centerDiff.y() != 0 )
	{
		int row = m_ctrl.sRow + centerDiff.y();
		int col = m_ctrl.sCol - centerDiff.x();

		if ( row < 0 || col < 0 || row > m_ctrl.sRow * 2 || col > m_ctrl.sCol * 2 )
		{
			qDebug("[%s] centerDiff - out of range.", Q_FUNC_INFO);
		}
		else
		{
			setRowCol(row, col);
		}
	}

	// TO-DO : need mirroring? call hflip() or vflip() here
	hflip(true);
	vflip(true);

	// start
	{
		int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (::ioctl(m_fdCam, VIDIOC_STREAMON, &type) == -1)
		{
			qDebug("[%s] Error - ioctl(camFile, VIDIOC_STREAMON, &type), errno - %d", Q_FUNC_INFO, errno);
			return false;
		}
	}

	locker.unlock();

#ifdef PUFF_SEP_CAM_CALIB_THREAD
	// run calib thread first
	m_calibThread.startThread(m_camWidth, m_camHeight, m_calibrationPosition);
#endif

	// call run();
	start(QThread::LowPriority);


	msleep(10);


#else
	Q_UNUSED(cam_size);
	Q_UNUSED(centerDiff);
#endif
	return true;
}

void CamThread::stopCameraThread()
{
#ifdef __arm_A20__

#ifdef PUFF_SEP_CAM_CALIB_THREAD
	m_calibThread.stop();
#endif

	QMutexLocker locker(&_mutex);

	if (m_runningFlag == false)
		return;

	_stopFlag = true;

	wait();

	{
		int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (::ioctl(m_fdCam, VIDIOC_STREAMOFF, &type) == -1)
		{
			qDebug("[%s] Error - ioctl(camFile, VIDIOC_STREAMOFF, &type)", Q_FUNC_INFO);
		}
	}

	for (int i = 0; i < CAM_STREAM_BUFFERS ; i++)
	{
		if (m_camData[i] && m_camDataSize)
		{
			::munmap(m_camData[i], m_camDataSize);
			msleep(_camDelay);
		}
		m_camData[i] = 0;
	}
	m_camDataSize = 0;
	m_camWidth = 0;
	m_camHeight = 0;
	m_camSize = 0;
#endif
}


void CamThread::run()
{
#ifdef __arm_A20__
	if (m_runningFlag)
		return;

	m_runningFlag = true;

	_stopFlag = false;

	camLoop();

	m_runningFlag = false;
#endif
}

int CamThread::initCameraDevice()
{			
#ifdef __arm_A20__
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
			qDebug("[%s] Error - open(CAM_DEVICE, O_RDONLY)", Q_FUNC_INFO);
			return 0;
		}
	}

	msleep(_camDelay);

	//-------------------------------------------------------------------------
	{
		int input = 0;

		if (::ioctl(camFile, VIDIOC_S_INPUT, &input) < 0)
		{
			qDebug("[%s] Error - ioctl(camFile, VIDIOC_S_INPUT, &input), errno - %d", Q_FUNC_INFO, errno);
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

		vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_GREY;

		vfmt.fmt.pix.priv = 0;
		vfmt.fmt.pix.field = V4L2_FIELD_NONE;

		if (::ioctl(camFile, VIDIOC_S_FMT, &vfmt) < 0)
		{
			qDebug("[%s] Error - VIDIOC_S_FMT, errno - %d", Q_FUNC_INFO, errno);
			goto INIT_CAM_ERROR;
		}
		else
		{
			qDebug("[%s] %d x %d", Q_FUNC_INFO, m_camWidth, m_camHeight);
		}
	}

	msleep(_camDelay);

	// Allocate buffer for captured image
	//-------------------------------------------------------------------------
	{
		struct v4l2_requestbuffers req;

		req.count = CAM_STREAM_BUFFERS;
		req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		req.memory = V4L2_MEMORY_MMAP;

		if (::ioctl(camFile, VIDIOC_REQBUFS, &req) < 0)
		{
			qDebug("[%s] Error - VIDIOC_REQBUFS, errno - %d", Q_FUNC_INFO, errno);
			goto INIT_CAM_ERROR;
		}

		if ( req.count != CAM_STREAM_BUFFERS )
		{
			qDebug("[%s] CAM_STREAM_BUFFERS = %d, req.count = %d",Q_FUNC_INFO, CAM_STREAM_BUFFERS, req.count);
			if ( req.count > CAM_STREAM_BUFFERS )
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
				qDebug("[%s] Error - VIDIOC_QUERYBUF, errno=%d", Q_FUNC_INFO, errno);
				qDebug("EBADF(%d),EINTR(%d),EINVAL(%d),EIO(%d),ENOTTY(%d),ENXIO(%d),ENODEV(%d)", EBADF,EINTR,EINVAL,EIO,ENOTTY,ENXIO,ENODEV);
				goto INIT_CAM_ERROR;
			}

			qDebug("[%s] VIDIOC_QUERYBUF - index: %d, length: %d, offset: %d", Q_FUNC_INFO, buf.index, buf.length, buf.m.offset);

			m_camData[loop] = (char *)mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camFile, buf.m.offset);
			m_camDataSize = buf.length;

			if (m_camData[loop] == MAP_FAILED)
			{
				qDebug("[%s] Error - mmap (%d)", Q_FUNC_INFO, errno);
				goto INIT_CAM_ERROR;
			}

			if ( -1 == ::ioctl(camFile, VIDIOC_QBUF, &buf))
			{
				qDebug("[%s] Error - VIDIOC_QBUF, errno - %d", Q_FUNC_INFO, errno);
			}
			qDebug("[%s] m_camData[%d] = %p, m_camDataSize = %d", Q_FUNC_INFO, loop, m_camData[loop], m_camDataSize);
		}
	}

	msleep(_camDelay);

	return camFile;

INIT_CAM_ERROR:
	if(camFile != 0) ::close(camFile);
	return 0;
#else

	return 0;
#endif
}

void CamThread::camLoop()
{
#ifdef __arm_A20__
	struct v4l2_buffer camBuffer;

#ifdef PUFF_SEP_CAM_CALIB_THREAD
	bool skipQBuf = false;
#endif

#ifdef PUFF_CAM_THREAD_CALIB
	bool processFrame = false;
#endif

	int ret;
	fd_set rdSet;
	struct timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	qDebug("[%s] start", Q_FUNC_INFO);

	QImage img(m_camWidth, m_camHeight, QImage::Format_Indexed8);
	img.setColorTable(m_yuv_rgb_conv_color_tab);

	QTime loopTimer;
	int loopCount = -1;
	int dispCount = 0;

	loopTimer.start();

	m_currentCamStreamBuffer = 0;

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
			++loopCount;


#ifdef PUFF_CAM_THREAD_CALIB
			// calib & disp 1/2
			if ( processFrame || m_camSize != SIZE_480s )
#endif
			{
#ifdef A20_CAM_THREAD_USE_DISP
				if ( m_streamOn && m_dispCam )
				{
					// call image compensation routine here : camera image size : m_camWidth * m_camHeight, camera image buffer : m_camData[camBuffer.index] (gray index 8 format)

					if ( m_showCamHideFB && (dispCount > 1) )
					{
						if ( !A20Disp::instance().hideLayer(A20Disp::LAYER_FB_BACK_BUF) )
						{
							qDebug("[%s] disp fbback layer hide error", Q_FUNC_INFO);
						}
						qDebug("DBG HIDE FB BACK");

						m_showCamHideFB = false;
					}

#ifdef PUFF_CAM_THREAD_CALIB
					calibrateImage(m_camData[camBuffer.index], img);
					A20Disp::instance().setCamBufAddr( camBuffer.m.offset );
					m_currentCamStreamBuffer = m_camData[camBuffer.index];
					++dispCount;
#endif
				}

#ifdef PUFF_SEP_CAM_CALIB_THREAD


#ifdef PUFF_DUMP_IMAGE
				m_calibThread.dispNSave(m_camData[camBuffer.index], camBuffer.m.offset);
#else

				struct CameraDisplayData item;

				item.camAddr = m_camData[camBuffer.index];
				item.camBuffer = camBuffer;
				item.dispUpdate = m_streamOn && m_dispCam;

				//qDebug("DBG #%d frame %p, idx %d, %s", loopCount, item.camAddr, camBuffer.index, qPrintable(QTime::currentTime().toString("hh:mm:ss zzz")) );

				if ( m_calibThread.calibNDisp(&item) )
					skipQBuf = true;
#endif // PUFF_DUMP_IMAGE

				dispCount = m_calibThread.getDispCount();
#endif


#endif // A20_CAM_THREAD_USE_DISP



#ifdef A20_CAM_THREAD_EMIT_SIGNAL
				emit signalCamStream(m_camData[camBuffer.index], camBuffer.m.offset);
#endif // A20_CAM_THREAD_EMIT_SIGNAL



#ifdef PUFF_CAM_THREAD_CALIB
				// capture
				if (_captureImage)
				{
					QMutexLocker captureLocker(&_captureMutex);

					if ( _captureImage->format() == QImage::Format_Indexed8 )
					{
						memcpy(_captureImage->bits(), m_camData[camBuffer.index], _captureImage->byteCount());
					}
					else
					{
						memcpy(img.bits(), m_camData[camBuffer.index], img.byteCount());
						memcpy(_captureImage->bits(), img.convertToFormat( _captureImage->format() ).bits(), _captureImage->byteCount() );
					}

					emit signalCaptureComplete(_captureImage);

					_captureImage = 0;
				}
#endif
			}

#ifdef PUFF_SEP_CAM_CALIB_THREAD


#ifdef PUFF_DUMP_IMAGE
#else
			QList <struct CameraDisplayData> doneList;

			if ( m_calibThread.getDoneList(doneList) && !doneList.isEmpty())
			{
				if (_captureImage)
				{
					QMutexLocker captureLocker(&_captureMutex);

					char * camAddr = doneList.last().camAddr;

					if ( _captureImage->format() == QImage::Format_Indexed8 )
					{
						memcpy(_captureImage->bits(), camAddr, _captureImage->byteCount());
					}
					else
					{
						memcpy(img.bits(), camAddr, img.byteCount());
						memcpy(_captureImage->bits(), img.convertToFormat( _captureImage->format() ).bits(), _captureImage->byteCount() );
					}

					emit signalCaptureComplete(_captureImage);

					_captureImage = 0;
				}


				for ( int i = 0 ; i < doneList.size(); ++i )
				{
					if ( -1 == ::ioctl(m_fdCam, VIDIOC_QBUF, &(doneList.at(i).camBuffer) ))
					{
						qDebug("[%s] VIDIOC_QBUF error %d", Q_FUNC_INFO, errno);
					}

					//qDebug("DBG done %p, idx %d, %s", doneList.at(i).camAddr, doneList.at(i).camBuffer.index, qPrintable(QTime::currentTime().toString("hh:mm:ss zzz")) );
				}
			}

			if ( skipQBuf )
			{
				skipQBuf = false;
				continue;
			}
#endif // PUFF_DUMP_IMAGE

#endif


#ifdef PUFF_CAM_THREAD_CALIB
			processFrame = !processFrame;
#endif

			if ( -1 == ::ioctl(m_fdCam, VIDIOC_QBUF, &camBuffer))
			{
				qDebug("[%s] VIDIOC_QBUF error %d", Q_FUNC_INFO, errno);
				continue;
			}
			//qDebug("DBG #%d queue %p, idx %d, %s", loopCount, m_camData[camBuffer.index], camBuffer.index, qPrintable(QTime::currentTime().toString("hh:mm:ss zzz")) );

		}
	}
	qDebug("[%s] end. %d , %d captured, %d msec elapsed. (%d)", Q_FUNC_INFO, dispCount * 1000 / loopTimer.elapsed(), dispCount, loopTimer.elapsed(), loopCount);

#ifdef PUFF_SEP_CAM_CALIB_THREAD

#ifdef PUFF_DUMP_IMAGE
#else
	QList <struct CameraDisplayData> doneList;

	if ( m_calibThread.getDoneList(doneList) )
	{
		for ( int i = 0 ; i < doneList.size(); ++i )
		{
			//qDebug("DBG remain %d, qbuf %d", i, doneList.at(i).camBuffer.index );
			if ( -1 == ::ioctl(m_fdCam, VIDIOC_QBUF, &(doneList.at(i).camBuffer) ))
			{
				qDebug("[%s] VIDIOC_QBUF error %d", Q_FUNC_INFO, errno);
			}
		}
	}
#endif // PUFF_DUMP_IMAGE

#endif

#endif // __arm_A20__
}

bool CamThread::isRunning()
{
	QMutexLocker locker(&_mutex);

	return m_runningFlag;
}

bool CamThread::s_ctrl(int id, int value)
{	
#ifdef __arm_A20__
	struct v4l2_control ctl;

	ctl.id = id;
	ctl.value = value;

	if ( ::ioctl(m_fdCam, VIDIOC_S_CTRL, &ctl) == 0 )
		return true;
	else
		return false;
#else
	Q_UNUSED(id);
	Q_UNUSED(value);
	return false;
#endif
}

bool CamThread::g_ctrl(int id, int &value)
{
#ifdef __arm_A20__
	struct v4l2_control ctl;

	ctl.id = id;

	if ( ::ioctl(m_fdCam, VIDIOC_G_CTRL, &ctl) == 0 )
	{
		value = ctl.value;
		return true;
	}
	else
		return false;
#else
	Q_UNUSED(id);
	Q_UNUSED(value);

	return false;
#endif
}

bool CamThread::setRowCol(int row, int col)
{
	if ( s_ctrl(V4L2_CID_AUDIO_BASS, row) && s_ctrl(V4L2_CID_AUDIO_TREBLE, col) )
	{
		m_ctrl.row = row;
		m_ctrl.col = col;
		qDebug("[%s] row = %d, col = %d", Q_FUNC_INFO, m_ctrl.row, m_ctrl.col);
		return true;
	}
	return false;
}

bool CamThread::setRow(int row)
{
	if ( s_ctrl(V4L2_CID_AUDIO_BASS, row) )
	{
		m_ctrl.row = row;
		qDebug("[%s] row = %d", Q_FUNC_INFO, m_ctrl.row);
		return true;
	}
	return false;
}

bool CamThread::setCol(int col)
{
	if ( s_ctrl(V4L2_CID_AUDIO_TREBLE, col) )
	{
		m_ctrl.col = col;
		qDebug("[%s] col = %d", Q_FUNC_INFO, m_ctrl.col);
		return true;
	}
	return false;
}

bool CamThread::adjRow(bool up, int step)
{
	return adj_v4l2_cid(V4L2_CID_AUDIO_BASS, m_ctrl.row, up, 0, MAX_ROW, "row", step);
}

bool CamThread::adjCol(bool up, int step)
{
	return adj_v4l2_cid(V4L2_CID_AUDIO_TREBLE, m_ctrl.col, up, 0, MAX_COL, "col", step);
}

bool CamThread::adjGain(bool up, int step)
{
	return adj_v4l2_cid(V4L2_CID_GAIN, m_ctrl.gain, up, 0, MAX_GAIN, "gain", step);
}

bool CamThread::adjExp(bool up, int step)
{
	return adj_v4l2_cid(V4L2_CID_EXPOSURE, m_ctrl.exp, up, 0, MAX_EXP, "exp", step);
}

bool CamThread::setGain( int value )
{
	return set_v4l2_cid(V4L2_CID_GAIN, m_ctrl.gain, 0, MAX_GAIN, "gain", value);
}

bool CamThread::setExp( int value )
{
	return set_v4l2_cid(V4L2_CID_EXPOSURE, m_ctrl.exp, 0, MAX_EXP, "exp", value);
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
				qDebug("[%s] %s = %d", Q_FUNC_INFO, pr_name, var);
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
				qDebug("[%s] %s = %d", Q_FUNC_INFO, pr_name, var);
				return true;
			}
		}
	}
	return false;
}

bool CamThread::set_v4l2_cid( int v4l2_id, int & var, int min, int max, const char *pr_name, int value )
{
	if ( value <= max && value >= min)
	{
		var = value;
		if ( s_ctrl(v4l2_id, var) )
		{
			qDebug("[%s] %s = %d", Q_FUNC_INFO, pr_name, var);
			return true;
		}
	}

	return false;
}

void CamThread::closeCameraDevice()
{
	stopCameraThread();

	QMutexLocker locker(&_mutex);

#ifndef WIN32
	if ( m_fdCam > 0 )
		::close(m_fdCam);
#endif

	m_fdCam = 0;

	if ( m_dispCam )
	{
		A20Disp::instance().quit();
		m_dispCam = false;
	}
}

bool CamThread::vflip(bool on)
{
	// working : kernel 3.4.104d

	if ( s_ctrl(V4L2_CID_VFLIP, on ? 1 : 0) )
	{
		return true;
	}
	else
	{
		qDebug("[%s] s_ctrl vflip %d err", Q_FUNC_INFO, on);
		return false;
	}
}

bool CamThread::hflip(bool on)
{
	// working : kernel 3.4.104d

	if ( s_ctrl(V4L2_CID_HFLIP, on ? 1 : 0) )
	{
		return true;
	}
	else
	{
		qDebug("[%s] s_ctrl hflip %d err", Q_FUNC_INFO, on);
		return false;
	}
}

bool CamThread::startCam(int cam_idx, const QPoint &centerDiff, int cam_size, bool clear_FB)
{
	Q_UNUSED(clear_FB);

	if ( cam_idx != ImageCamera )
	{
		qDebug("[%s] invalid cam_idx.", Q_FUNC_INFO);
		return false;
	}

	int prevCamSize = m_startCamSize;

	enum todoFlags
	{
		TODO_SHOW = 0x01,
		TODO_RESET = 0x02,
		TODO_STREAM = 0x04,
	};
	int toDo = 0;

	if ( isRunning() )
	{
		if ( prevCamSize == cam_size )
		{
			qDebug("[%s] same size %d, keep previous camera thread.", Q_FUNC_INFO, cam_size);
			return true;
		}

		setStreamOn(false);

		toDo |= TODO_STREAM;
	}

	int camThreadSize = cam_size;
	if ( cam_size == CAM_SIZE_x1_5 || cam_size == CAM_SIZE_x1_3 )
	{
		camThreadSize = SIZE_480s;
	}

	if ( !startCameraThread(camThreadSize , centerDiff) )
		return false;

	m_startCamSize = cam_size;
	m_dispScaling = false;
	m_scalingSrcSize = 0;
	if ( m_startCamSize == CAM_SIZE_x1_5 || m_startCamSize == CAM_SIZE_x1_3 )
	{
		m_dispScaling = true;
		m_scalingSrcSize = m_startCamSize / 2;
		qDebug("[%s] cam size %d, apply disp scaling.", Q_FUNC_INFO, m_startCamSize);
	}

	if ( m_dispCam == false )
	{
		if ( A20Disp::instance().init() )
		{
			if ( !(A20Disp::instance().addCamBufLayer(m_camWidth, m_camHeight, m_topLeftPos.x(), m_topLeftPos.y(), m_dispSize.x(), m_dispSize.y(), m_dispScaling) &&
				 A20Disp::instance().addFbBackBufLayer(m_camWidth, m_camHeight, m_topLeftPos.x(), m_topLeftPos.y(), m_dispSize.x(), m_dispSize.y()) ) )
			{
				 qDebug("[%s] layer add error", Q_FUNC_INFO);
				 return false;
			}
			m_dispCam = true;
			toDo = TODO_SHOW;

			if ( m_dispScaling )
				toDo |= TODO_RESET;
		}
		else
		{
			qDebug("[%s] disp init error", Q_FUNC_INFO);
			return false;
		}
	}
	else
	{
		if ( prevCamSize != m_startCamSize )
		{
			toDo |= TODO_RESET;
		}

		if ( !A20Disp::instance().isLayerVisible(A20Disp::LAYER_CAM_BUF) )
		{
			toDo |= TODO_SHOW;
		}
	}

	if ( toDo & TODO_RESET )
	{
		if ( m_dispScaling )
		{
			if ( !A20Disp::instance().setLayerWindowEx(A20Disp::LAYER_CAM_BUF, m_camWidth, m_camHeight, (m_camWidth - m_scalingSrcSize)/2, (m_camHeight - m_scalingSrcSize)/2, m_scalingSrcSize, m_scalingSrcSize, m_topLeftPos.x(), m_topLeftPos.y(), m_dispSize.x(), m_dispSize.y()) )
			{
				qDebug("[%s] disp cam layer window ex error", Q_FUNC_INFO);
				return false;
			}
		}
		else
		{
			if ( !A20Disp::instance().setLayerWindow(A20Disp::LAYER_CAM_BUF, m_camWidth, m_camHeight, m_topLeftPos.x(), m_topLeftPos.y(), m_dispSize.x(), m_dispSize.y()) )
			{
				qDebug("[%s] disp cam layer window error", Q_FUNC_INFO);
				return false;
			}
		}
	}

	if ( toDo & TODO_SHOW )
	{
		if ( !(A20Disp::instance().showLayer(A20Disp::LAYER_CAM_BUF) &&
			A20Disp::instance().enableColorKey(A20Disp::LAYER_CAM_BUF)) )
		{
			qDebug("[%s] disp cam layer show error", Q_FUNC_INFO);
			return false;
		}
	}

	if ( toDo & TODO_STREAM )
	{
		setStreamOn(true);
	}
	return true;
}

void CamThread::stopCam()
{
	stopCameraThread();
	m_startCamSize = 0;
}

void CamThread::setTopLeftOnFB(const QPoint &topleft_pos)
{
	m_topLeftPos = topleft_pos;

	if ( m_dispCam )
	{
		if ( A20Disp::instance().moveLayer(A20Disp::LAYER_CAM_BUF, m_topLeftPos.x(), m_topLeftPos.y(), m_dispSize.x(), m_dispSize.y()) == false)
			qDebug("[%s] cam layer move error - %d, %d,  %d, %d", Q_FUNC_INFO, m_topLeftPos.x(), m_topLeftPos.y(), m_camWidth, m_camHeight);
	}
}

bool CamThread::moveCam(int camPosX, int camPosY)
{
	QMutexLocker locker(&_mutex);

	if ( camPosX > m_ctrl.sCol )
	{
		camPosX = m_ctrl.sCol;
		qDebug("[%s] camPosX - out of range. %d", Q_FUNC_INFO, camPosX);
	}
	else if ( camPosX < -m_ctrl.sCol )
	{
		camPosX = -m_ctrl.sCol;
		qDebug("[%s] camPosX - out of range. %d", Q_FUNC_INFO, camPosX);
	}

	if ( camPosY > m_ctrl.sRow )
	{
		camPosY = m_ctrl.sRow;
		qDebug("[%s] camPosY - out of range. %d", Q_FUNC_INFO, camPosY);
	}
	else if ( camPosY < -m_ctrl.sRow )
	{
		camPosY = -m_ctrl.sRow;
		qDebug("[%s] camPosY - out of range. %d", Q_FUNC_INFO, camPosY);
	}

	return setRowCol(m_ctrl.sRow + camPosY, m_ctrl.sCol - camPosX);
}

bool CamThread::captureCam(QImage *image)
{
#ifdef __arm_A20__	
	{
		QMutexLocker locker(&_mutex);

		if (m_runningFlag == false)
			return false;
	}

	if( image->width() != m_camWidth )
	{
		qDebug("[%s] Image & Capture camera size is different. IMAGE(%dx%d),CAM(%dx%d)", Q_FUNC_INFO, image->width(), image->height(), m_camWidth, m_camHeight);
		return false;
	}

	QMutexLocker captureLocker(&_captureMutex);

	if (_captureImage)
		return false;

	_captureImage = image;

	return true;
#else
	Q_UNUSED(image);

	return false;
#endif
}

bool CamThread::captureCam(QImage *image, int cam_index, const QPoint &cam_center_diff, int cam_size)
{
	if ( cam_index != ImageCamera )
	{
		qDebug("[%s] cam_index %d not supported.", Q_FUNC_INFO, cam_index);
		return false;
	}

	if ( size2wh(cam_size, 0, 0) == false )
	{
		qDebug("[%s] cam_size %d not supported.", Q_FUNC_INFO, cam_size);
		return false;
	}

	if( image->width() != cam_size )
	{
		qDebug("[%s] Image & Capture camera size is different. IMAGE(%dx%d),CAM(%d)", Q_FUNC_INFO, image->width(), image->height(), cam_size);
		return false;
	}

	if(save())
	{
		setStreamOn(false);

		bool started = true;

		if(m_runningFlag)
		{
			if( m_camWidth != cam_size )
				started = startCameraThread(cam_size, cam_center_diff);
			else
				return captureCam(image);
		}
		else
		{
			started = startCameraThread(cam_size, cam_center_diff);
		}

		if ( started )
		{
			_captureImage = image;
			return true;
		}
		else
		{
			setStreamOn(m_saved.streamOn);
			m_saved.saved = false;
		}
	}

	qDebug("[%s] Saving camera status is failed.", Q_FUNC_INFO);
	return false;
}

bool CamThread::save()
{
	if ( m_saved.saved )
		return false;

	m_saved.saved = true;
	m_saved.width = m_camWidth;
	m_saved.streamOn = m_streamOn;
	m_saved.centerDiff = getCenterDiff();

	return true;
}

void CamThread::restore()
{
	if ( m_saved.saved )
	{
		if ( m_saved.width == 0 )
		{
			stopCameraThread();
		}
		else
		{
			startCameraThread(m_saved.width, m_saved.centerDiff);
		}
		setStreamOn(m_saved.streamOn);
		m_saved.saved = false;
	}
}

QImage CamThread::combineCameraAndFrameBuffer(const QImage & imgCam, const QImage & imgFb, bool scaling, int scalingSrcSize)
{
	QImage src = imgCam;

	if ( scaling )
	{
		if ( scalingSrcSize != src.width() || scalingSrcSize != src.height() )
			src = src.copy( (src.width() - m_dispSize.x())/2, (src.height() - m_dispSize.y())/2, scalingSrcSize, scalingSrcSize);

		src = src.scaled(m_dispSize.x(), m_dispSize.y());
	}
	else
	{
		if ( src.width() != m_dispSize.x() || src.height() != m_dispSize.y() )
		{
			QRect rect;

			if ( src.width() > m_dispSize.x() )
			{
				rect.setX( (src.width() - m_dispSize.x())/2 );
				rect.setWidth( m_dispSize.x() );
			}
			else
			{
				rect.setX(0);
				rect.setWidth(src.width());

			}
			if ( src.height() > m_dispSize.y() )
			{
				rect.setY( (src.height() - m_dispSize.y())/2 );
				rect.setHeight( m_dispSize.y());
			}
			else
			{
				rect.setY(0);
				rect.setHeight(src.height());
			}
			src = src.copy(rect);
		}
	}

	src = src.convertToFormat(QImage::Format_RGB16);

	QImage target = imgFb.convertToFormat(QImage::Format_RGB16);

	int x, y;

	for ( y = m_topLeftPos.y() ; y < m_topLeftPos.y() + src.height(); ++y )
	{
		for ( x = m_topLeftPos.x(); x < m_topLeftPos.x() + src.width(); ++x )
		{
			if ( target.pixel(x, y) == qRgb(0, 255, 0) )
			{
				target.setPixel(x, y, src.pixel( x - m_topLeftPos.x(), y - m_topLeftPos.y()));
			}
		}
	}
	return target;
}

void CamThread::slotCaptureComplete(QImage *image)
{
	restore();

	if ( m_imageScreenShot && (image == m_imageScreenShot) )
	{
		QString prefix = screenShotDir + "/sc";

		if ( screenShotDateTime )
			prefix += QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

		QString path = prefix + ".png";

		QImage imgFb = A20Disp::instance().saveFB(A20Disp::FB_MAIN);

		combineCameraAndFrameBuffer(*m_imageScreenShot, imgFb, m_dispScaling, m_scalingSrcSize ).save(path);
		qDebug("[CamThread::slotCaptureComplete] screenshot saved. [%s]", qPrintable(path));

		if ( m_screenShotType == SC_ALL || m_screenShotType == SC_960_ALL )
		{
			m_imageScreenShot->save(prefix + "_cam.png");
			imgFb.save(prefix + "_fb.png");
			qDebug("[CamThread::slotCaptureComplete] camera/framebuffer image saved. [%s_fb, %s_cam.png]", qPrintable(prefix), qPrintable(prefix));
		}

		delete m_imageScreenShot;
		m_imageScreenShot = 0;
		m_screenShotType = SC_NONE;
	}
	else
	{
		emit imageCapture(image);
	}
}

void CamThread::setCalibrationData( QVector< QVector<QPointF> >& calibrationPosition )
{
	QMutexLocker locker(&_compensationMutex);
	m_calibrationPosition = calibrationPosition;

#ifdef PUFF_SEP_CAM_CALIB_THREAD
	m_calibThread.setCalibrationData(calibrationPosition);
#endif
}

#ifdef PUFF_CAM_THREAD_CALIB
void CamThread::calibrateImage(char* distortedImage, QImage& tempImage)
{	
	QMutexLocker locker(&_compensationMutex);

	if (m_bCompensation == false || m_camWidth != m_calibrationPosition.size())
		return;
	
	const int offset = 20;
	const int originalImageSize = m_camWidth;
	const int xCount = originalImageSize - offset;
	const int yCount = originalImageSize - offset;
	uchar* tempImageStart = tempImage.bits();

	memcpy(tempImageStart, distortedImage, tempImage.byteCount());

	for (int i = offset; i < yCount; i++)
	{
		for (int j = offset; j < xCount; j++)
		{
			QPointF& residual = m_calibrationPosition[i][j];

			float xPosition = residual.x();
			float yPosition = residual.y();

			int x1 = xPosition;
			int x2 = x1 + 1;
			int y1 = yPosition;
			int y2 = y1 + 1;

			int gray11 = *(tempImageStart + originalImageSize * y1 + x1);
			int gray21 = *(tempImageStart + originalImageSize * y1 + x2);
			int gray12 = *(tempImageStart + originalImageSize * y2 + x1);
			int gray22 = *(tempImageStart + originalImageSize * y2 + x2);

			int grayTop = (gray21 - gray11) * (xPosition - x1) + gray11;
			int grayBottom = (gray22 - gray12) * (xPosition - x1) + gray12;

			int grayLevel = (grayBottom - grayTop) * (yPosition - y1) + grayTop;

			*(distortedImage + i * originalImageSize + j) = grayLevel;
		}
	}
}
#endif


void CamThread::slotUnixSignal(int signal)
{
	if ( signal == 2) // SIGINT
	{
		if ( m_dispCam )
		{
			qDebug("[CamThread::slotUnixSignal] quit disp engine.");
			A20Disp::instance().quit();
		}
		return;
	}
	if ( !(signal == SC_NORMAL || signal == SC_ALL || signal == SC_960_ALL ) )
		return;

	if ( m_imageScreenShot || _captureImage )
	{
		qDebug("[CamThread::slotUnixSignal] Capture in progress... %p %p", m_imageScreenShot, _captureImage);
		return;
	}

	if ( !isRunning() && signal == SC_NORMAL )
	{
		// frame buffer screenshot only
		QString path = screenShotDir + "/sc";

		if ( screenShotDateTime )
			path += QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

		path += ".png";

		if ( !m_dispCam )
			A20Disp::instance().init(); // init only

		A20Disp::instance().saveFB(A20Disp::FB_MAIN).convertToFormat(QImage::Format_RGB16).save(path);
		qDebug("[CamThread::slotUnixSignal] frame buffer screenshot saved. [%s]", qPrintable(path));
		return;
	}

	int w = m_camWidth;
	int h = m_camHeight;

	if ( signal == SC_960_ALL )
	{
		size2wh(SIZE_960, &w, &h);
	}

	m_imageScreenShot = new QImage(w, h, QImage::Format_Indexed8);
	if ( m_imageScreenShot )
	{
		m_imageScreenShot->setColorTable(m_yuv_rgb_conv_color_tab);

		m_screenShotType = signal;
		if ( signal == SC_960_ALL )
		{
			captureCam(m_imageScreenShot, ImageCamera, getCenterDiff(), SIZE_960);
		}
		else
		{
			captureCam(m_imageScreenShot);
		}
	}
}

void CamThread::setCompensationOn( bool bCompensation )
{
	m_bCompensation = bCompensation;

#ifdef PUFF_SEP_CAM_CALIB_THREAD
	m_calibThread.setCompensationOn(bCompensation);
#endif
}

void CamThread::setDispStreamOn(bool flag)
{
	if ( flag )
	{
		m_showCamHideFB = true;
	}
	else
	{

#ifdef PUFF_SEP_CAM_CALIB_THREAD
		m_currentCamStreamBuffer = m_calibThread.getLastDispBuf();
#endif

		if ( m_currentCamStreamBuffer == 0 )
		{
			qDebug("[%s] cambuf err", Q_FUNC_INFO);
			return;
		}

		QImage img(m_camWidth, m_camHeight, QImage::Format_Indexed8);
		img.setColorTable(m_yuv_rgb_conv_color_tab);
		::memcpy(img.bits(), m_currentCamStreamBuffer, img.byteCount());

		if ( !A20Disp::instance().setLayerWindow(A20Disp::LAYER_FB_BACK_BUF, m_dispSize.x(), m_dispSize.y(), m_topLeftPos.x(), m_topLeftPos.y(), m_dispSize.x(), m_dispSize.y()) )
		{
			qDebug("[%s] disp fbback layer window error", Q_FUNC_INFO);
			return;
		}

		if ( m_dispScaling )
		{
			if ( m_scalingSrcSize != m_camWidth || m_scalingSrcSize != m_camHeight )
			{
				A20Disp::instance().drawCam(0, 0, (unsigned char *)img.copy((m_camWidth - m_scalingSrcSize)/2, (m_camHeight - m_scalingSrcSize)/2, m_scalingSrcSize, m_scalingSrcSize).scaled(m_dispSize.x(), m_dispSize.y()).convertToFormat(QImage::Format_RGB32).bits(), m_dispSize.x(), m_dispSize.y(), A20Disp::FB_BACK);
			}
			else
			{
				A20Disp::instance().drawCam(0, 0, (unsigned char *)img.scaled(m_dispSize.x(), m_dispSize.y()).convertToFormat(QImage::Format_RGB32).bits(), m_dispSize.x(), m_dispSize.y(), A20Disp::FB_BACK);
			}
		}
		else
		{
			if ( m_dispSize.x() == m_camWidth && m_dispSize.y() == m_camHeight )
			{
				A20Disp::instance().drawCam(0, 0, (unsigned char *)img.convertToFormat(QImage::Format_RGB32).bits(), m_camWidth, m_camHeight, A20Disp::FB_BACK);
			}
			else
			{
				QRect rect;

				if ( m_camWidth > m_dispSize.x() )
				{
					rect.setX( (m_camWidth - m_dispSize.x())/2 );
					rect.setWidth( m_dispSize.x() );
				}
				else
				{
					rect.setX( 0 );
					rect.setWidth( m_camWidth );
				}

				if ( m_camHeight > m_dispSize.y() )
				{
					rect.setY( (m_camHeight - m_dispSize.y())/2 );
					rect.setHeight( m_dispSize.y() );
				}
				else
				{
					rect.setY( 0 );
					rect.setHeight(m_camHeight);
				}
				A20Disp::instance().drawCam(0, 0, (unsigned char *) img.copy(rect).convertToFormat(QImage::Format_RGB32).bits(), m_dispSize.x(), m_dispSize.y(), A20Disp::FB_BACK);
			}
		}

		if ( !A20Disp::instance().showLayer(A20Disp::LAYER_FB_BACK_BUF) )
			qDebug("[%s] disp fbback layer show error", Q_FUNC_INFO);

		if ( !A20Disp::instance().enableColorKey(A20Disp::LAYER_FB_BACK_BUF) )
			qDebug("[%s] disp fbback layer colorkey error", Q_FUNC_INFO);

		if ( !A20Disp::instance().toBottom(A20Disp::LAYER_CAM_BUF) )
			qDebug("[%s] disp cam layer bottom error", Q_FUNC_INFO);

		qDebug("DBG SHOW FB BACK");
	}
}

void CamThread::setStreamOn(bool flag)
{
	if ( m_dispCam && (m_streamOn != flag) )
		setDispStreamOn(flag);

	m_streamOn = flag;
}
