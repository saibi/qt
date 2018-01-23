#include "a20disp.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <errno.h>

#define DISP_DEVICE "/dev/disp"
#define FB_DEVICE "/dev/fb0"


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

	::memset(&m_screen, 0, sizeof(m_screen));
	::memset(m_layer, 0, sizeof(m_layer));
}

bool A20Disp::checkDispVer()
{
	if ( m_fd.disp >= 0 )
	{
		int tmp;

		tmp = SUNXI_DISP_VERSION;
		if (::ioctl(m_fd.disp, DISP_CMD_VERSION, &tmp) < 0)
			return false;
		return true;
	}
	return false;
}

bool A20Disp::init()
{
	if ( m_fd.fb >= 0 || m_fd.disp >= 0 )
	{
		qDebug("[%s] already started.", Q_FUNC_INFO);
		return false;
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
	return true;
}

void A20Disp::quit()
{
	if ( m_fd.fb < 0 || m_fd.disp < 0 )
		return;

	if ( m_layer[LAYER_CAM_FB].id !=0 )
		removeLayer(LAYER_CAM_FB);
	if ( m_layer[LAYER_CAM_SCALER].id != 0 )
		removeLayer(LAYER_CAM_SCALER);

	::munmap(m_screen.fb_addr, m_screen.fb_fix.smem_len);

	if ( m_fd.fb >= 0 )
		::close(m_fd.fb);

	if ( m_fd.disp >= 0 )
		::close(m_fd.disp);

	clearVars();
}


bool A20Disp::addCamScalerLayer(int camWidth, int camHeight, int x, int y, int w, int h, int idx)
{
	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = DISP_LAYER_WORK_MODE_SCALER;

	m_layer[idx].id = ::ioctl(m_fd.disp, DISP_CMD_LAYER_REQUEST, args);
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
	m_layer[idx].param.src_win.x       = 0;
	m_layer[idx].param.src_win.y       = 0;
	m_layer[idx].param.src_win.width   = camWidth;
	m_layer[idx].param.src_win.height  = camHeight;
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
	return true;
}

bool A20Disp::addCamFbLayer(int x, int y, int w, int h, int idx)
{
	if ( m_fd.disp < 0 )
	{
		qDebug("[%s] init disp first", Q_FUNC_INFO);
		return false;
	}


	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = DISP_LAYER_WORK_MODE_NORMAL;

	m_layer[idx].id = ::ioctl(m_fd.disp, DISP_CMD_LAYER_REQUEST, args);
	if ( m_layer[idx].id <= 0 )
	{
		qDebug("[%s] layer request error", Q_FUNC_INFO);
		return false;
	}
	qDebug("[%s] #%d id = %d", Q_FUNC_INFO, idx, m_layer[idx].id);

#if 0
	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	args[2] = (unsigned int)&(m_layer[idx].param);
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_GET_PARA, args) < 0)
	{
		qDebug("[%s] layer get_para error", Q_FUNC_INFO);
		m_layer[idx].id = 0;
		return false;
	}
#endif

	m_layer[idx].param.mode            = DISP_LAYER_WORK_MODE_NORMAL;
	m_layer[idx].param.pipe            = 1;
	m_layer[idx].param.prio            = 0;
	m_layer[idx].param.alpha_en        = 1;
	m_layer[idx].param.alpha_val       = 0xff;
	m_layer[idx].param.ck_enable       = 0;
	m_layer[idx].param.src_win.x       = 0;
	m_layer[idx].param.src_win.y       = 0;
	m_layer[idx].param.src_win.width   = m_screen.fb_var.xres;
	m_layer[idx].param.src_win.height  = m_screen.fb_var.yres;
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
	return true;
}

bool A20Disp::showLayer(int idx)
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

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	::ioctl(m_fd.disp, DISP_CMD_LAYER_TOP, args);

	if ( idx == LAYER_CAM_SCALER )
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

	return true;
}

bool A20Disp::hideLayer(int idx)
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
	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;

	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_CLOSE, args) != 0 )
	{
		qDebug("[%s] layer close error", Q_FUNC_INFO);
		return false;
	}

	if ( idx == LAYER_CAM_SCALER )
		::ioctl(m_fd.disp, DISP_CMD_VIDEO_STOP, args);

	return true;
}


bool A20Disp::removeLayer(int idx)
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
	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;

	if ( ::ioctl(m_fd.disp, DISP_CMD_LAYER_RELEASE, args) != 0 )
	{
		qDebug("[%s] layer release error", Q_FUNC_INFO);
		return false;
	}
	m_layer[idx].id = 0;
	return true;
}


void A20Disp::setCamScalerAddr(unsigned int addr, int idx)
{
	// assert : fd & layer set properly
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
}

bool A20Disp::moveLayer(int idx, int x, int y, int width, int height)
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
}

bool A20Disp::enableColorKey(int idx, unsigned int color)
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
}

bool A20Disp::disableColorKey(int idx)
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

	unsigned int args[4];

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_CK_OFF, args) < 0)
	{
		qDebug("[%s] layer ck off error", Q_FUNC_INFO);
		return false;
	}

	args[0] = SCREEN_0;
	args[1] = m_layer[idx].id;
	if (::ioctl(m_fd.disp, DISP_CMD_LAYER_TOP, args) < 0)
	{
		qDebug("[%s] layer top error", Q_FUNC_INFO);
		return false;
	}
	return true;
}


void A20Disp::drawImg(int x, int y, const QImage &img, enum FrameBufferAreaIndex fbIdx)
{
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
}

void A20Disp::fillBox(int x, int y, int w, int h, unsigned int c, FrameBufferAreaIndex fbIdx)
{
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
}

bool A20Disp::toggleFbArea(FrameBufferAreaIndex fbIdx)
{
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
}

void A20Disp::copyFb(FrameBufferAreaIndex srcIdx)
{
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

}

void A20Disp::drawCam(int x, int y, unsigned char *camData, int width, int height, enum FrameBufferAreaIndex fbIdx)
{
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
}
