#include "disp.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <errno.h>

#define DISP_DEVICE "/dev/disp"


Disp::Disp(QObject *parent) : QObject(parent)
{
	clearMemberVars();
}

Disp::~Disp()
{
}

void Disp::clearMemberVars()
{
	m_fdDisp = 0;
	m_layer = 0;
	m_screenHeight = 0;
	m_screenWidth = 0;

	::memset(&m_layerParam, 0, sizeof(__disp_layer_info_t));
}

int Disp::wrap_ioctl(int fd, unsigned long req, unsigned int *addr)
{
	int ret = ::ioctl(fd, req, addr);
	if ( ret < 0 )
		qDebug("[%s] ret = %d, errno = %d", Q_FUNC_INFO, ret, errno);

	return ret;
}

bool Disp::init(int x, int y, int width, int height)
{
	if ( m_fdDisp > 0 )
	{
		qDebug("[%s] already initialized.", Q_FUNC_INFO);
		return false;
	}

	if ( (m_fdDisp = ::open(DISP_DEVICE, O_RDWR)) < 0 )
	{
		qDebug("[%s] %s open error", Q_FUNC_INFO, DISP_DEVICE);
		return false;
	}
	qDebug("[%s] %s open ok", Q_FUNC_INFO, DISP_DEVICE);


	unsigned int args[4];

	args[0] = 0;
	m_screenWidth = wrap_ioctl(m_fdDisp, DISP_CMD_SCN_GET_WIDTH, args);
	m_screenHeight = wrap_ioctl(m_fdDisp, DISP_CMD_SCN_GET_HEIGHT, args);
	qDebug("m_screenWidth = %d, m_screenHeight = %d", m_screenWidth, m_screenHeight);

	args[0] = 0;
	args[1] = DISP_LAYER_WORK_MODE_SCALER;
	m_layer = wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_REQUEST, args);
	if (m_layer == 0)
	{
	   qDebug("request layer failed.");
	   return false;
	}
	qDebug("m_layer = %d", m_layer);

	m_layerParam.mode            = DISP_LAYER_WORK_MODE_SCALER;
	m_layerParam.pipe            = 1;
	m_layerParam.prio            = 0;
	m_layerParam.alpha_en        = 1;
	m_layerParam.alpha_val       = 0xff;
	m_layerParam.ck_enable       = 0;
	m_layerParam.src_win.x       = 0;
	m_layerParam.src_win.y       = 0;
	m_layerParam.src_win.width   = 0;
	m_layerParam.src_win.height  = 0;
	m_layerParam.scn_win.x       = x;
	m_layerParam.scn_win.y       = y;
	m_layerParam.scn_win.width   = width;
	m_layerParam.scn_win.height  = height;
	m_layerParam.fb.addr[0]      = 0;
	m_layerParam.fb.addr[1]      = 0;
	m_layerParam.fb.addr[2]      = 0;
	m_layerParam.fb.size.width   = 0;
	m_layerParam.fb.size.height  = 0;
	m_layerParam.fb.format       = DISP_FORMAT_ARGB8888;
	m_layerParam.fb.seq          = DISP_SEQ_ARGB;
	m_layerParam.fb.mode         = DISP_MOD_NON_MB_PLANAR;
	m_layerParam.fb.br_swap      = 0;
	args[0] = 0;
	args[1] = m_layer;
	args[2] = (__u32)&m_layerParam;
	wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_SET_PARA, args);

	args[0] = 0;
	args[1] = m_layer;
	wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_TOP, args);

	return true;
}

void Disp::set_para(int width, int height)
{
	if ( m_fdDisp <= 0 )
	{
		qDebug("[%s] disp not initialized.", Q_FUNC_INFO);
		return;
	}

	unsigned int args[4];

	m_layerParam.src_win.width   = width;
	m_layerParam.src_win.height  = height;
	m_layerParam.fb.size.width   = width;
	m_layerParam.fb.size.height  = height;
	args[0] = 0;
	args[1] = m_layer;
	args[2] = (__u32)&m_layerParam;
	wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_SET_PARA, args);
}

void Disp::start()
{
	if ( m_fdDisp <= 0 )
	{
		qDebug("[%s] disp not initialized.", Q_FUNC_INFO);
		return;
	}

	unsigned int args[4];

	args[0] = 0;
	args[1] = m_layer;
	wrap_ioctl(m_fdDisp, DISP_CMD_VIDEO_START, args);
	wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_OPEN, args);
}

void Disp::stop()
{
	if ( m_fdDisp <= 0 )
	{
		qDebug("[%s] disp not initialized.", Q_FUNC_INFO);
		return;
	}

	unsigned int args[4];

	args[0] = 0;
	args[1] = m_layer;
	wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_CLOSE, args);
	wrap_ioctl(m_fdDisp, DISP_CMD_VIDEO_STOP, args);
}

void Disp::quit()
{
	if ( m_fdDisp > 0 )
	{
		unsigned int args[4];

		if ( m_layer > 0 )
		{
			args[0] = 0;
			args[1] = m_layer;
			wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_RELEASE, args);
		}

		::close(m_fdDisp);

		qDebug("[%s] disp closed", Q_FUNC_INFO);
	}

	clearMemberVars();
}

int Disp::set_addr(int width, int height, unsigned int *addr)
{
	if ( m_fdDisp <= 0 )
	{
		qDebug("[%s] disp not initialized.", Q_FUNC_INFO);
		return -1;
	}
	int csi_format = V4L2_PIX_FMT_GREY;

	__disp_video_fb_t fb;
	unsigned int args[4];

	::memset(&fb, 0, sizeof(__disp_video_fb_t));

	fb.interlace       = 0;
	fb.top_field_first = 0;
	fb.frame_rate      = 24;
	fb.addr[0]         = *addr;
	fb.id              = 0;

	switch (csi_format)
	{

	case V4L2_PIX_FMT_YUV422P:
	case V4L2_PIX_FMT_YUYV:
	case V4L2_PIX_FMT_YVYU:
	case V4L2_PIX_FMT_UYVY:
	case V4L2_PIX_FMT_VYUY:
		fb.addr[1] = *addr + width * height;
		fb.addr[2] = *addr + width * height * 3 / 2;
		break;

	case V4L2_PIX_FMT_YUV420:
		fb.addr[1] = *addr + width * height;
		fb.addr[2] = *addr + width * height * 5 / 4;
		break;

	case V4L2_PIX_FMT_NV16:
	case V4L2_PIX_FMT_NV12:
	case V4L2_PIX_FMT_HM12:
		fb.addr[1] = *addr + width * height;
		fb.addr[2] = m_layerParam.fb.addr[1];
		break;

	case V4L2_PIX_FMT_GREY:
		fb.addr[1] = *addr;
		fb.addr[2] = *addr;

		//fb.addr[1] = *addr + width * height;
		//fb.addr[2] = m_layerParam.fb.addr[1];
		break;

	default:
		break;
	}

	args[0] = 0;
	args[1] = m_layer;
	args[2] = (__u32)&fb;
	return wrap_ioctl(m_fdDisp, DISP_CMD_VIDEO_SET_FB, args);
}


bool Disp::enableColorKey(int screenLayer, unsigned int color)
{
	if ( m_fdDisp <= 0 )
	{
		qDebug("[%s] disp not initialized.", Q_FUNC_INFO);
		return false;
	}

	unsigned int args[4];

	__disp_colorkey_t colorkey;
	__disp_color_t disp_color;

	disp_color.alpha = (color >> 24) & 0xFF;
	disp_color.red   = (color >> 16) & 0xFF;
	disp_color.green = (color >> 8)  & 0xFF;
	disp_color.blue  = (color >> 0)  & 0xFF;

	colorkey.ck_min = disp_color;
	colorkey.ck_max = disp_color;
	colorkey.red_match_rule   = 2;
	colorkey.green_match_rule = 2;
	colorkey.blue_match_rule  = 2;

	args[0] = 0;
	args[1] = (unsigned int)&colorkey;
	if (wrap_ioctl(m_fdDisp, DISP_CMD_SET_COLORKEY, args) )
		return false;

	/* Set the overlay layer below the screen layer */
	args[0] = 0;
	args[1] = screenLayer;
	if (wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_BOTTOM, args) < 0)
		return false;

	args[0] = 0;
	args[1] = m_layer;
	if (wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_BOTTOM, args) < 0)
		return false;

	/* Enable color key for the overlay layer */
	args[0] = 0;
	args[1] = m_layer;
	if (wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_CK_ON, args) < 0)
		return false;

	/* Disable color key and enable global alpha for the screen layer */
	args[0] = 0;
	args[1] = screenLayer;
	if (wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_CK_OFF, args) < 0)
		return false;

	args[0] = 0;
	args[1] = screenLayer;
	if (wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_ALPHA_ON, args) < 0)
		return false;

	return true;
}

bool Disp::disableColorKey()
{
	if ( m_fdDisp <= 0 )
	{
		qDebug("[%s] disp not initialized.", Q_FUNC_INFO);
		return false;
	}

	unsigned int args[4];

	/* Disable color key for the overlay layer */
	args[0] = 0;
	args[1] = m_layer;
	if (wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_CK_OFF, args) < 0)
		return false;

	/* Set the overlay layer above the screen layer */
	args[0] = 0;
	args[1] = m_layer;
	if (wrap_ioctl(m_fdDisp, DISP_CMD_LAYER_TOP, args) < 0)
		return false;

	return true;
}
