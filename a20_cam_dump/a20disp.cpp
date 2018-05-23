#include "a20disp.h"

#ifdef __linux__
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <errno.h>
#endif

#define DISP_DEVICE "/dev/disp"
#define FB_DEVICE "/dev/fb0"
#define G2D_DEVICE "/dev/g2d"


A20Disp::A20Disp(QObject *parent) : QObject(parent)
{
	clearVars();
}

A20Disp::~A20Disp()
{
}

void A20Disp::clearVars()
{
	m_fd.fb = -1;
	m_fd.disp = -1;
#ifdef A20_G2D_TEST
	m_fd.g2d = -1;
#endif

	::memset(&m_screen, 0, sizeof(m_screen));
	::memset(m_layer, 0, sizeof(m_layer));

	m_scalerUsed = false;
}

bool A20Disp::checkDispVer()
{
#ifdef __arm_A20__
	if ( m_fd.disp >= 0 )
	{
		int tmp;

		tmp = SUNXI_DISP_VERSION;
		if (::ioctl(m_fd.disp, DISP_CMD_VERSION, &tmp) < 0)
			return false;
		return true;
	}
#endif
	return false;
}

bool A20Disp::init()
{
#ifdef __arm_A20__
	if ( m_fd.fb >= 0 || m_fd.disp >= 0 )
	{
		qDebug("[%s] already started.", Q_FUNC_INFO);
		return true;
	}

	m_fd.disp = ::open(DISP_DEVICE, O_RDWR);
	if (m_fd.disp < 0 )
	{
		qDebug("[%s] %s open error", Q_FUNC_INFO, DISP_DEVICE);
		return false;
	}

	if ( ! checkDispVer() )
	{
		::close(m_fd.disp);
		m_fd.disp = -1;
		qDebug("[%s] disp version error", Q_FUNC_INFO);
		return false;
	}

	m_fd.fb = ::open(FB_DEVICE, O_RDWR);
	if (m_fd.fb < 0)
	{
		::close(m_fd.disp);
		m_fd.disp = -1;
		qDebug("[%s] %s open error", Q_FUNC_INFO, FB_DEVICE);
		return false;
	}

	if (::ioctl(m_fd.fb, FBIOGET_VSCREENINFO, &m_screen.fb_var) < 0 ||
		::ioctl(m_fd.fb, FBIOGET_FSCREENINFO, &m_screen.fb_fix) < 0)
	{
		::close(m_fd.fb);
		m_fd.fb = -1;
		::close(m_fd.disp);
		m_fd.disp = -1;

		qDebug("[%s] FBIOGET error", Q_FUNC_INFO);
		return false;
	}
	m_screen.fbDepthBytes = m_screen.fb_var.bits_per_pixel / 8;

	m_screen.fb_addr = (char *)::mmap(0, m_screen.fb_fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd.fb, 0);
	if ((char *) -1 == m_screen.fb_addr )
	{
		::close(m_fd.fb);
		m_fd.fb = -1;
		::close(m_fd.disp);
		m_fd.disp = -1;

		qDebug("[%s] framebuffer mmap error", Q_FUNC_INFO);
		return false;
	}

	if (::ioctl(m_fd.fb, FBIOGET_LAYER_HDL_0, &m_screen.layerId))
	{
		::close(m_fd.fb);
		m_fd.fb = -1;
		::close(m_fd.disp);
		m_fd.disp = -1;

		qDebug("[%s] screen layer id error", Q_FUNC_INFO);
		return false;
	}

	qDebug("[%s] %d %d * %d * %d\n%p, %d, %p", Q_FUNC_INFO, m_screen.layerId, m_screen.fb_var.xres, m_screen.fb_var.yres, m_screen.fb_var.bits_per_pixel,
		   (void*)m_screen.fb_fix.smem_start, m_screen.fb_fix.smem_len, m_screen.fb_addr);

#ifdef A20_G2D_TEST
	// g2d test
	m_fd.g2d = ::open(G2D_DEVICE, O_RDWR);
	if ( m_fd.g2d < 0 )
	{
		qDebug("[%s] g2d device error", Q_FUNC_INFO);
	}
#endif

	return true;
#else
	return false;
#endif
}

void A20Disp::quit()
{
#ifdef __arm_A20__
	if ( m_fd.fb < 0 || m_fd.disp < 0 )
		return;

	if ( m_layer[LAYER_FB_BACK_BUF].id !=0 )
	{
		hideLayer(LAYER_FB_BACK_BUF);
		removeLayer(LAYER_FB_BACK_BUF);
	}
	if ( m_layer[LAYER_CAM_BUF].id != 0 )
	{
		hideLayer(LAYER_CAM_BUF);
		removeLayer(LAYER_CAM_BUF);
	}

	::munmap(m_screen.fb_addr, m_screen.fb_fix.smem_len);

	if ( m_fd.fb >= 0 )
		::close(m_fd.fb);

	if ( m_fd.disp >= 0 )
		::close(m_fd.disp);

#ifdef A20_G2D_TEST
	if ( m_fd.g2d >= 0 )
		::close(m_fd.g2d);
#endif


#endif // __arm_A20__
	clearVars();
}

bool A20Disp::addCamBufLayer(int camWidth, int camHeight, int x, int y, int w, int h, bool scaling, int idx)
{
#ifdef __arm_A20__
	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_scalerUsed )
	{
		qDebug("[%s] remove scaler layer first", Q_FUNC_INFO);
		return false;
	}

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = DISP_LAYER_WORK_MODE_SCALER;

	m_layer[idx].id = ::ioctl(m_fd.disp, DISP_CMD_LAYER_REQUEST, args);
	m_layer[idx].flag = 0;
	if ( m_layer[idx].id <= 0 )
	{
		qDebug("[%s] layer request error", Q_FUNC_INFO);
		return false;
	}
	qDebug("[%s] #%d id = %d", Q_FUNC_INFO, idx, m_layer[idx].id);

	m_layer[idx].param.mode            = DISP_LAYER_WORK_MODE_SCALER;
	m_layer[idx].param.pipe            = 1;
	m_layer[idx].param.prio            = 0;
	m_layer[idx].param.alpha_en        = 1;
	m_layer[idx].param.alpha_val       = 0xff;
	m_layer[idx].param.ck_enable       = 0;
	if ( scaling )
	{
		m_layer[idx].param.src_win.x       = 0;
		m_layer[idx].param.src_win.y       = 0;
		m_layer[idx].param.src_win.width   = camWidth;
		m_layer[idx].param.src_win.height  = camHeight;
	}
	else
	{
		m_layer[idx].param.src_win.x       = (camWidth > w) ? (camWidth - w)/2 : 0;
		m_layer[idx].param.src_win.y       = (camHeight > h) ? (camHeight - h)/2 : 0;
		m_layer[idx].param.src_win.width   = (camWidth > w) ? w : camWidth;
		m_layer[idx].param.src_win.height  = (camHeight > h) ? h : camHeight;
	}
	m_layer[idx].param.scn_win.x       = x;
	m_layer[idx].param.scn_win.y       = y;
	m_layer[idx].param.scn_win.width   = w;
	m_layer[idx].param.scn_win.height  = h;
	m_layer[idx].param.fb.addr[0]      = 0;
	m_layer[idx].param.fb.addr[1]      = 0;
	m_layer[idx].param.fb.addr[2]      = 0;
	m_layer[idx].param.fb.size.width   = camWidth;
	m_layer[idx].param.fb.size.height  = camHeight;
	m_layer[idx].param.fb.format       = DISP_FORMAT_ARGB8888;
	m_layer[idx].param.fb.seq          = DISP_SEQ_ARGB;
	m_layer[idx].param.fb.mode         = DISP_MOD_NON_MB_PLANAR;
	m_layer[idx].param.fb.br_swap      = 0;

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	args[2] = (unsigned int)&(m_layer[idx].param);
	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_SET_PARA, args) != 0 )
	{
		qDebug("[%s] layer set para error", Q_FUNC_INFO);
		m_layer[idx].id = 0;
		return false;
	}

	m_scalerUsed = true;
	return true;
#else
	Q_UNUSED(camHeight);
	Q_UNUSED(camWidth);
	Q_UNUSED(x);
	Q_UNUSED(y);
	Q_UNUSED(w);
	Q_UNUSED(h);
	Q_UNUSED(scaling);
	Q_UNUSED(idx);

	return false;
#endif
}

bool A20Disp::addFbBackBufLayer(int camWidth, int camHeight, int x, int y, int w, int h, bool forceScaler, int idx)
{
#ifdef __arm_A20__

	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	int reqMode = DISP_LAYER_WORK_MODE_NORMAL;

	if ( forceScaler )
	{
		if ( m_scalerUsed )
		{
			qDebug("[%s] remove scaler layer first", Q_FUNC_INFO);
			return false;
		}

		reqMode = DISP_LAYER_WORK_MODE_SCALER;
	}

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = reqMode;

	m_layer[idx].id = ::ioctl(m_fd.disp, DISP_CMD_LAYER_REQUEST, args);
	m_layer[idx].flag = 0;
	if ( m_layer[idx].id <= 0 )
	{
		qDebug("[%s] layer request error", Q_FUNC_INFO);
		return false;
	}
	qDebug("[%s] #%d id = %d", Q_FUNC_INFO, idx, m_layer[idx].id);

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	args[2] = (unsigned int)&(m_layer[idx].param);
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_GET_PARA, args) < 0)
	{
		qDebug("[%s] layer get_para error", Q_FUNC_INFO);
		m_layer[idx].id = 0;
		return false;
	}

	m_layer[idx].param.mode            =  (__disp_layer_work_mode_t)reqMode;
	m_layer[idx].param.pipe            = 1;
	m_layer[idx].param.prio            = 0;
	m_layer[idx].param.alpha_en        = 1;
	m_layer[idx].param.alpha_val       = 0xff;
	m_layer[idx].param.ck_enable       = 0;
	if ( forceScaler )
	{
		m_layer[idx].param.src_win.x       = 0;
		m_layer[idx].param.src_win.y       = 0;
		m_layer[idx].param.src_win.width   = camWidth;
		m_layer[idx].param.src_win.height  = camHeight;
	}
	else
	{
		m_layer[idx].param.src_win.x       = (camWidth > w) ? (camWidth - w)/2 : 0;
		m_layer[idx].param.src_win.y       = (camHeight > h) ? (camHeight - h)/2 : 0;
		m_layer[idx].param.src_win.width   = (camWidth > w) ? w : camWidth;
		m_layer[idx].param.src_win.height  = (camHeight > h) ? h : camHeight;
	}
	m_layer[idx].param.scn_win.x       = x;
	m_layer[idx].param.scn_win.y       = y;
	m_layer[idx].param.scn_win.width   = w;
	m_layer[idx].param.scn_win.height  = h;
	m_layer[idx].param.fb.addr[0]      = m_screen.fb_fix.smem_start + (m_screen.fb_var.xres * m_screen.fb_var.yres * m_screen.fbDepthBytes);
	m_layer[idx].param.fb.size.width   = m_screen.fb_var.xres;
	m_layer[idx].param.fb.size.height  = m_screen.fb_var.yres;
	m_layer[idx].param.fb.format       = DISP_FORMAT_ARGB8888;
	m_layer[idx].param.fb.seq          = DISP_SEQ_ARGB;
	m_layer[idx].param.fb.mode         = DISP_MOD_INTERLEAVED;
	m_layer[idx].param.fb.br_swap      = 0;

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	args[2] = (unsigned int)&(m_layer[idx].param);
	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_SET_PARA, args) != 0 )
	{
		qDebug("[%s] layer set para error", Q_FUNC_INFO);
		m_layer[idx].id = 0;
		return false;
	}

	if ( forceScaler )
		m_scalerUsed = true;

	return true;
#else
	Q_UNUSED(camWidth);
	Q_UNUSED(camHeight);
	Q_UNUSED(x);
	Q_UNUSED(y);
	Q_UNUSED(w);
	Q_UNUSED(h);
	Q_UNUSED(forceScaler);
	Q_UNUSED(idx);

	return false;
#endif
}

bool A20Disp::showLayer(int idx, bool toBottom)
{
#ifdef __arm_A20__

	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_layer[idx].id == 0 )
	{
		qDebug("[%s] add layer first", Q_FUNC_INFO);
		return false;
	}

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	::ioctl(m_fd.disp, toBottom ? DISP_CMD_LAYER_BOTTOM : DISP_CMD_LAYER_TOP, args);

	if ( idx == LAYER_CAM_BUF )
	{
		args[0] = SCREEN_0;
		args[1] = m_layer[idx].id;

		::ioctl(m_fd.disp, DISP_CMD_VIDEO_START, args);
	}

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;

	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_OPEN, args) != 0 )
	{
		qDebug("[%s] layer open error", Q_FUNC_INFO);
		return false;
	}

	m_layer[idx].flag |= LAYER_FLAG_SHOW;

	return true;
#else
	Q_UNUSED(idx);
	Q_UNUSED(toBottom);
	return false;
#endif
}

bool A20Disp::hideLayer(int idx)
{
#ifdef __arm_A20__

	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_layer[idx].id == 0 )
	{
		qDebug("[%s] add layer first", Q_FUNC_INFO);
		return false;
	}
	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;

	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_CLOSE, args) != 0 )
	{
		qDebug("[%s] layer close error", Q_FUNC_INFO);
		return false;
	}

	if ( idx == LAYER_CAM_BUF )
	{
		args[0] = SCREEN_0;
		args[1] = m_layer[idx].id;
		::ioctl(m_fd.disp, DISP_CMD_VIDEO_STOP, args);
	}

	m_layer[idx].flag &= (~LAYER_FLAG_SHOW);

	return true;
#else
	Q_UNUSED(idx);

	return false;
#endif
}


bool A20Disp::removeLayer(int idx)
{
#ifdef __arm_A20__

	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_layer[idx].id == 0 )
	{
		qDebug("[%s] add layer first", Q_FUNC_INFO);
		return false;
	}
	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;

	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_RELEASE, args) != 0 )
	{
		qDebug("[%s] layer release error", Q_FUNC_INFO);
		return false;
	}
	m_layer[idx].id = 0;
	m_layer[idx].flag = 0;

	if ( m_layer[idx].param.mode == DISP_LAYER_WORK_MODE_SCALER )
		m_scalerUsed = false;

	return true;
#else
	Q_UNUSED(idx);
	return false;
#endif
}


void A20Disp::setCamBufAddr(unsigned int addr, int idx)
{
#ifdef __arm_A20__
	if ( m_fd.disp < 0 || m_layer[idx].id == 0)
		return ;

	__disp_video_fb_t fb;

	::memset(&fb, 0, sizeof(__disp_video_fb_t));

	fb.interlace       = 0;
	fb.top_field_first = 0;
	fb.frame_rate      = 24;
	fb.addr[0]         = addr;
	fb.addr[1]         = addr;
	fb.addr[2]         = addr;
	fb.id              = 0;

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	args[2] = (unsigned int)&fb;
	::ioctl(m_fd.disp, DISP_CMD_VIDEO_SET_FB, args);
#else
	Q_UNUSED(addr);
	Q_UNUSED(idx);
#endif
}

bool A20Disp::moveLayer(int idx, int x, int y, int width, int height)
{
#ifdef __arm_A20__

	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_layer[idx].id == 0 )
	{
		qDebug("[%s] add layer first", Q_FUNC_INFO);
		return false;
	}
	unsigned int args[4];

	m_layer[idx].param.scn_win.x       = x;
	m_layer[idx].param.scn_win.y       = y;
	m_layer[idx].param.scn_win.width   = width;
	m_layer[idx].param.scn_win.height  = height;

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	args[2] = (unsigned int)&(m_layer[idx].param);
	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_SET_PARA, args) != 0 )
	{
		qDebug("[%s] layer set para", Q_FUNC_INFO);
		return false;
	}
	return true;
#else
	Q_UNUSED(idx);
	Q_UNUSED(x);
	Q_UNUSED(y);
	Q_UNUSED(width);
	Q_UNUSED(height);

	return false;
#endif
}

bool A20Disp::enableColorKey(int idx, unsigned int color)
{
#ifdef __arm_A20__

	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_layer[idx].id == 0 )
	{
		qDebug("[%s] add layer first", Q_FUNC_INFO);
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

	args[0] = SCREEN_0;
	args[1] = (unsigned int)&colorkey;
	if (::ioctl(m_fd.disp, DISP_CMD_SET_COLORKEY, args) )
	{
		qDebug("[%s] set colorkey error", Q_FUNC_INFO);
		return false;
	}

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_BOTTOM, args) < 0)
	{
		qDebug("[%s] layer bottom error", Q_FUNC_INFO);
		return false;
	}

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_CK_ON, args) < 0)
	{
		qDebug("[%s] layer ck on error", Q_FUNC_INFO);
		return false;
	}
	m_layer[idx].param.ck_enable = 1;

	args[0] = SCREEN_0;
	args[1] = m_screen.layerId;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_CK_OFF, args) < 0)
	{
		qDebug("[%s] layer ck off error", Q_FUNC_INFO);
		return false;
	}

	args[0] = SCREEN_0;
	args[1] = m_screen.layerId;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_ALPHA_ON, args) < 0)
	{
		qDebug("[%s] layer alpha on error", Q_FUNC_INFO);
		return false;
	}

	return true;
#else
	Q_UNUSED(idx);
	Q_UNUSED(color);
	return false;
#endif
}



bool A20Disp::disableColorKey(int idx)
{
#ifdef __arm_A20__

	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_layer[idx].id == 0 )
	{
		qDebug("[%s] add layer first", Q_FUNC_INFO);
		return false;
	}

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_CK_OFF, args) < 0)
	{
		qDebug("[%s] layer ck off error", Q_FUNC_INFO);
		return false;
	}
	m_layer[idx].param.ck_enable = 0;

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_TOP, args) < 0)
	{
		qDebug("[%s] layer top error", Q_FUNC_INFO);
		return false;
	}
	return true;
#else
	Q_UNUSED(idx);

	return false;
#endif
}

bool A20Disp::isLayerAdded(int idx)
{
#ifdef __arm_A20__
	if ( m_fd.disp < 0 )
	{
		return false;
	}
	if ( m_layer[idx].id == 0 )
	{
		return false;
	}
	return true;
#else
	Q_UNUSED(idx);

	return false;
#endif
}

bool A20Disp::toBottom(int idx)
{
#ifdef __arm_A20__
	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_layer[idx].id == 0 )
	{
		qDebug("[%s] add layer first", Q_FUNC_INFO);
		return false;
	}

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_BOTTOM, args) < 0)
	{
		qDebug("[%s] layer bottom error", Q_FUNC_INFO);
		return false;
	}
	return true;
#else
	Q_UNUSED(idx);

	return false;
#endif
}

bool A20Disp::setLayerWindowEx(int idx, int camWidth, int camHeight, int srcX, int srcY, int srcW, int srcH, int dispX, int dispY, int dispW, int dispH, bool checkFd)
{
#ifdef __arm_A20__
	if ( checkFd )
	{
		if ( m_fd.disp < 0 )
		{
			qDebug("[%s] init disp first", Q_FUNC_INFO);
			return false;
		}

		if ( m_layer[idx].id == 0 )
		{
			qDebug("[%s] add layer first", Q_FUNC_INFO);
			return false;
		}
	}

	unsigned int args[4];

	m_layer[idx].param.src_win.x       = srcX;
	m_layer[idx].param.src_win.y       = srcY;
	m_layer[idx].param.src_win.width   = srcW;
	m_layer[idx].param.src_win.height  = srcH;

	m_layer[idx].param.scn_win.x       = dispX;
	m_layer[idx].param.scn_win.y       = dispY;
	m_layer[idx].param.scn_win.width   = dispW;
	m_layer[idx].param.scn_win.height  = dispH;

	if ( idx == LAYER_CAM_BUF )
	{
		m_layer[idx].param.fb.addr[0]      = 0;
		m_layer[idx].param.fb.addr[1]      = 0;
		m_layer[idx].param.fb.addr[2]      = 0;
		m_layer[idx].param.fb.size.width   = camWidth;
		m_layer[idx].param.fb.size.height  = camHeight;
	}

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	args[2] = (unsigned int)&(m_layer[idx].param);
	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_SET_PARA, args) != 0 )
	{
		qDebug("[%s] layer set para error", Q_FUNC_INFO);
		return false;
	}
	return true;
#else
	Q_UNUSED(idx);
	Q_UNUSED(camHeight);
	Q_UNUSED(camWidth);
	Q_UNUSED(srcX);
	Q_UNUSED(srcY);
	Q_UNUSED(srcW);
	Q_UNUSED(srcH);
	Q_UNUSED(dispX);
	Q_UNUSED(dispY);
	Q_UNUSED(dispW);
	Q_UNUSED(dispH);
	Q_UNUSED(checkFd);

	return false;
#endif
}

bool A20Disp::setLayerWindow(int idx, int camWidth, int camHeight, int x, int y, int w, int h, bool scaling, bool checkFd)
{
	if ( scaling )
		return setLayerWindowEx(idx, camWidth, camHeight, 0, 0, camWidth, camHeight, x, y, w, h, checkFd);
	else
		return setLayerWindowEx(idx, camWidth, camHeight, (camWidth > w) ? (camWidth - w)/2 : 0, (camHeight > h) ? (camHeight - h)/2 : 0, (camWidth > w) ? w : camWidth, (camHeight > h) ? h : camHeight, x, y, w, h, checkFd);
}

bool A20Disp::layerVideoStartStop(bool flag, int idx)
{
#ifdef __arm_A20__
	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	if ( m_layer[idx].id == 0 )
	{
		qDebug("[%s] add layer first", Q_FUNC_INFO);
		return false;
	}

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;

	if ( ::ioctl(m_fd.disp, flag ? DISP_CMD_VIDEO_START : DISP_CMD_VIDEO_STOP , args) != 0 )
	{
		qDebug("[%s] layer video start/stop error", Q_FUNC_INFO);
		return false;
	}
	return true;
#else
	Q_UNUSED(idx);
	Q_UNUSED(flag);
	return false;
#endif
}










#ifdef A20_FB_TEST

void A20Disp::drawImg(int x, int y, const QImage &img, enum FrameBufferAreaIndex fbIdx)
{
#ifdef __arm_A20__

	if ( m_fd.fb < 0 )
	{
		qDebug("[%s] init fb first", Q_FUNC_INFO);
		return;
	}

	QImage *p = (QImage *)&img;
	QImage tmp;
	if ( p->format() != QImage::Format_RGB32 )
	{
		tmp = img.convertToFormat(QImage::Format_RGB32);
		p = &tmp;
	}

	unsigned int location;

	if ( fbIdx == FB_CUR)
		location = (x + m_screen.fb_var.xoffset) * m_screen.fbDepthBytes  + (y + m_screen.fb_var.yoffset) * m_screen.fb_fix.line_length;
	else
		location = (x + m_screen.fb_var.xoffset) * m_screen.fbDepthBytes  + (y + m_screen.fb_var.yres * fbIdx) * m_screen.fb_fix.line_length;

	for ( int idx = 0; idx < p->height() ; ++idx)
	{
		::memcpy( m_screen.fb_addr  + location, p->scanLine(idx), p->width() * m_screen.fbDepthBytes );
		location += m_screen.fb_fix.line_length;
	}
#else
	Q_UNUSED(x);
	Q_UNUSED(y);
	Q_UNUSED(img);
	Q_UNUSED(fbIdx);
#endif
}

void A20Disp::fillBox(int x, int y, int w, int h, unsigned int c, FrameBufferAreaIndex fbIdx)
{
#ifdef __arm_A20__

	if ( m_fd.fb < 0 )
	{
		qDebug("[%s] init fb first", Q_FUNC_INFO);
		return;
	}

	char * framebuffer = m_screen.fb_addr; ;
	unsigned int location;

	if ( fbIdx == FB_CUR)
		location = (x + m_screen.fb_var.xoffset) * m_screen.fbDepthBytes + (y + m_screen.fb_var.yoffset) * m_screen.fb_fix.line_length;
	else
		location = (x + m_screen.fb_var.xoffset) * m_screen.fbDepthBytes + (y + m_screen.fb_var.yres * fbIdx) * m_screen.fb_fix.line_length;

	for ( int iy = 0 ; iy < h; ++iy )
	{
		int hoffset = 0;
		for ( int ix = 0; ix < w; ++ ix )
		{
			*(framebuffer + location + hoffset) = c & 0xff; // blue
			*(framebuffer + location + hoffset + 1) = (c & 0xff00) >> 8; // green
			*(framebuffer + location + hoffset + 2) = (c & 0xff0000) >> 16; // red
			*(framebuffer + location + hoffset + 3) = (c & 0xff000000) >> 24; // alpha
			hoffset += m_screen.fbDepthBytes;
		}
		location += m_screen.fb_fix.line_length;
	}
#else
	Q_UNUSED(x);
	Q_UNUSED(y);
	Q_UNUSED(w);
	Q_UNUSED(h);
	Q_UNUSED(c);
	Q_UNUSED(fbIdx);
#endif
}

bool A20Disp::toggleFbArea(FrameBufferAreaIndex fbIdx)
{
#ifdef __arm_A20__

	if ( m_fd.fb < 0 )
	{
		qDebug("[%s] init fb first", Q_FUNC_INFO);
		return false;
	}

	if ( fbIdx == FB_MAIN )
		m_screen.fb_var.yoffset = 0;
	else if ( fbIdx == FB_BACK )
		m_screen.fb_var.yoffset = m_screen.fb_var.yres;
	else
	{
		if ( m_screen.fb_var.yoffset == 0 )
			m_screen.fb_var.yoffset = m_screen.fb_var.yres;
		else
			m_screen.fb_var.yoffset = 0;
	}

#if 0
	int dummy = 0;
	if (::ioctl(m_fd.fb, FBIO_WAITFORVSYNC, &dummy))
	{
		qDebug("[%s] FBIO_WAITFORVSYNC error", Q_FUNC_INFO);
		return false;
	}
#endif

	if (::ioctl(m_fd.fb, FBIOPAN_DISPLAY, &m_screen.fb_var) )
	{
		qDebug("[%s] FBIOPAN_DISPLAY error", Q_FUNC_INFO);
		return false;
	}

	return true;
#else
	Q_UNUSED(fbIdx);
	return false;
#endif
}

void A20Disp::copyFb(FrameBufferAreaIndex srcIdx)
{
#ifdef __arm_A20__

	if ( m_fd.fb < 0 )
	{
		qDebug("[%s] init fb first", Q_FUNC_INFO);
		return;
	}

	if ( srcIdx == FB_CUR )
	{
		if ( m_screen.fb_var.yoffset == 0 )
			srcIdx = FB_MAIN;
		else
			srcIdx = FB_BACK;
	}

	int screenSize = m_screen.fb_var.xres * m_screen.fb_var.yres * m_screen.fbDepthBytes;

	if ( srcIdx == FB_MAIN )
		::memcpy(m_screen.fb_addr + screenSize, m_screen.fb_addr, screenSize);
	else
		::memcpy(m_screen.fb_addr, m_screen.fb_addr + screenSize, screenSize);
#else
	Q_UNUSED(srcIdx);
#endif
}
#endif // A20_FB_TEST


void A20Disp::drawCam(int x, int y, unsigned char *camData, int width, int height, enum FrameBufferAreaIndex fbIdx)
{
#ifdef __arm_A20__

	if ( m_fd.fb < 0 )
	{
		qDebug("[%s] init fb first", Q_FUNC_INFO);
		return;
	}

	char *fbData;

	if ( fbIdx == FB_CUR )
		fbData = m_screen.fb_addr + ( x + m_screen.fb_var.xoffset) * m_screen.fbDepthBytes + ( y + m_screen.fb_var.yoffset) * m_screen.fb_fix.line_length;
	else
		fbData = m_screen.fb_addr + ( x + m_screen.fb_var.xoffset) * m_screen.fbDepthBytes + ( y + m_screen.fb_var.yres * fbIdx) * m_screen.fb_fix.line_length;

	int copyBytes = width * m_screen.fbDepthBytes;

	for ( int idx = 0 ; idx < height ; ++idx )
	{
		if ( (y + idx) >= int(m_screen.fb_var.yres) )
			break;

		memcpy(fbData, camData, copyBytes);
		fbData += m_screen.fb_fix.line_length;
		camData += copyBytes;
	}
#else
	Q_UNUSED(x);
	Q_UNUSED(y);
	Q_UNUSED(camData);
	Q_UNUSED(width);
	Q_UNUSED(height);
	Q_UNUSED(fbIdx);
#endif
}

QImage A20Disp::saveFB(FrameBufferAreaIndex srcIdx)
{
#ifdef __arm_A20__
	if ( m_fd.fb < 0 )
	{
		qDebug("[%s] init fb first", Q_FUNC_INFO);
		return QImage();
	}

	if ( srcIdx == FB_CUR )
	{
		if ( m_screen.fb_var.yoffset == 0 )
			srcIdx = FB_MAIN;
		else
			srcIdx = FB_BACK;
	}

	int screenSize = m_screen.fb_var.xres * m_screen.fb_var.yres * m_screen.fbDepthBytes;

	QImage img(m_screen.fb_var.xres, m_screen.fb_var.yres, QImage::Format_RGB32);

	if ( srcIdx == FB_MAIN )
		::memcpy(img.bits(), m_screen.fb_addr, screenSize);
	else
		::memcpy(img.bits(), m_screen.fb_addr + screenSize, screenSize);

	return img;
#else
	Q_UNUSED(srcIdx);

	return QImage();
#endif
}

















#ifdef DBG_LAYER_PARAM

void A20Disp::pr_layer_param(__disp_layer_info_t *p)
{
	qDebug("mode = %d", p->mode);
	qDebug("pipe = %d", p->pipe);
	qDebug("prio = %d", p->prio);
	qDebug("alpha_en = %d", p->alpha_en);
	qDebug("alpha_val = %d", p->alpha_val);
	qDebug("ck_enable = %d", p->ck_enable);
}


void A20Disp::dumpLayerParam(int idx)
{
#ifdef __arm_A20__
	unsigned int args[4];

	__disp_layer_info_t param;

	param = m_layer[idx].param;

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	args[2] = (unsigned int)&param;

	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_GET_PARA, args) < 0)
	{
		qDebug("[%s] layer get_para error", Q_FUNC_INFO);
	}
	else
	{
		qDebug("DBG before");
		pr_layer_param(&m_layer[idx].param);

		qDebug("DBG after");
		pr_layer_param(&param);
	}
#else
	Q_UNUSED(idx);
#endif
}
#endif // DBG_LAYER_PARAM

#ifdef A20_G2D_TEST

#include "g2d_driver.h"

bool A20Disp::g2d_fill(int x, int y, int w, int h, unsigned int color)
{

	g2d_fillrect tmp;

	if (m_fd.g2d < 0)
	{
		qDebug("[%s] g2d device err", Q_FUNC_INFO);
		return false;
	}

	if (w <= 0 || h <= 0)
	{
		qDebug("[%s] invalid w %d, h %d", Q_FUNC_INFO, w, h);
		return false;
	}

	tmp.flag                = G2D_FIL_NONE;
	tmp.dst_image.addr[0]   = m_screen.fb_fix.smem_start;
	tmp.dst_image.w         = m_screen.fb_var.xres;
	tmp.dst_image.h         = m_screen.fb_fix.smem_len / (m_screen.fb_var.xres * m_screen.fb_var.bits_per_pixel / 8 );
	tmp.dst_image.format    = G2D_FMT_ARGB_AYUV8888;
	tmp.dst_image.pixel_seq = G2D_SEQ_NORMAL;
	tmp.dst_rect.x          = x;
	tmp.dst_rect.y          = y;
	tmp.dst_rect.w          = w;
	tmp.dst_rect.h          = h;
	tmp.color               = color;
	tmp.alpha               = 0;

	if ( ::ioctl(m_fd.g2d, G2D_CMD_FILLRECT, &tmp) < 0 )
	{
		qDebug("[%s] G2D_CMD_FILLRECT error", Q_FUNC_INFO);
		return false;
	}
	return true;
}

bool A20Disp::g2d_blit(int dest_x, int dest_y, int src_x, int src_y, int w, int h)
{
	g2d_blt tmp;

	if (m_fd.g2d < 0)
	{
		qDebug("[%s] g2d device err", Q_FUNC_INFO);
		return false;
	}

	if (w <= 0 || h <= 0)
	{
		qDebug("[%s] invalid w %d, h %d", Q_FUNC_INFO, w, h);
		return false;
	}

	tmp.flag                = G2D_BLT_NONE;
	tmp.src_image.addr[0]   = m_screen.fb_fix.smem_start;
	tmp.src_image.w         = m_screen.fb_var.xres;
	tmp.src_image.h         = m_screen.fb_fix.smem_len / (m_screen.fb_var.xres * m_screen.fb_var.bits_per_pixel / 8 );
	tmp.src_image.format    = G2D_FMT_ARGB_AYUV8888;
	tmp.src_image.pixel_seq = G2D_SEQ_NORMAL;
	tmp.src_rect.x          = src_x;
	tmp.src_rect.y          = src_y;
	tmp.src_rect.w          = w;
	tmp.src_rect.h          = h;
	tmp.dst_image.addr[0]   = m_screen.fb_fix.smem_start;
	tmp.dst_image.w         = m_screen.fb_var.xres;
	tmp.dst_image.h         = m_screen.fb_fix.smem_len / (m_screen.fb_var.xres * m_screen.fb_var.bits_per_pixel / 8 );
	tmp.dst_image.format    = G2D_FMT_ARGB_AYUV8888;
	tmp.dst_image.pixel_seq = G2D_SEQ_NORMAL;
	tmp.dst_x               = dest_x;
	tmp.dst_y               = dest_y;
	tmp.color               = 0;
	tmp.alpha               = 0;

	if ( ::ioctl(m_fd.g2d, G2D_CMD_BITBLT, &tmp) < 0 )
	{
		qDebug("[%s] G2D_CMD_BITBLT error", Q_FUNC_INFO);
		return false;
	}
	return true;
}
#endif // A20_G2D_TEST
