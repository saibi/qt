#include "sunxidisp.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <errno.h>

SunxiDisp::SunxiDisp(QObject *parent) : QObject(parent)
{
	m_ctx = 0;
}

SunxiDisp::~SunxiDisp()
{
}

#define FB_DEVICE "/dev/fb0"
#define DISP_DEVICE "/dev/disp"

bool SunxiDisp::init()
{
	if ( m_ctx )
	{
		qDebug("[%s] already initialized.", Q_FUNC_INFO);
		return false;
	}

	m_ctx = new sunxi_disp_t;
	Q_CHECK_PTR(m_ctx);

	::memset(m_ctx, 0, sizeof(sunxi_disp_t));

	m_ctx->fd_disp = ::open(DISP_DEVICE, O_RDWR);

	/* maybe it's even not a sunxi hardware */
	if (m_ctx->fd_disp < 0) {
		delete m_ctx;
		m_ctx = 0;

		qDebug("[%s] /dev/disp open error", Q_FUNC_INFO);
		return false;
	}

	/* version check */
	int tmp, version;

	tmp = SUNXI_DISP_VERSION;
	version = ::ioctl(m_ctx->fd_disp, DISP_CMD_VERSION, &tmp);
	if (version < 0) {
		::close(m_ctx->fd_disp);
		delete m_ctx;
		m_ctx = 0;

		qDebug("[%s] /dev/disp version error", Q_FUNC_INFO);
		return false;
	}

	/* use /dev/fb0 by default */
	m_ctx->fb_id = 0;

	m_ctx->fd_fb = ::open(FB_DEVICE, O_RDWR);
	if (m_ctx->fd_fb < 0) {
		::close(m_ctx->fd_disp);
		delete m_ctx;
		m_ctx = 0;

		qDebug("[%s] /dev/fb0 open error", Q_FUNC_INFO);
		return false;
	}

	struct fb_var_screeninfo fb_var;
	struct fb_fix_screeninfo fb_fix;

	if (::ioctl(m_ctx->fd_fb, FBIOGET_VSCREENINFO, &fb_var) < 0 ||
		::ioctl(m_ctx->fd_fb, FBIOGET_FSCREENINFO, &fb_fix) < 0)
	{
		::close(m_ctx->fd_fb);
		::close(m_ctx->fd_disp);
		delete m_ctx;
		m_ctx = 0;

		qDebug("[%s] /dev/fb0 FBIOGET error", Q_FUNC_INFO);
		return false;
	}

	m_ctx->xres = fb_var.xres;
	m_ctx->yres = fb_var.yres;
	m_ctx->bits_per_pixel = fb_var.bits_per_pixel;
	m_ctx->framebuffer_paddr = fb_fix.smem_start;
	m_ctx->framebuffer_size = fb_fix.smem_len;
	m_ctx->framebuffer_height = m_ctx->framebuffer_size /
							  (m_ctx->xres * m_ctx->bits_per_pixel / 8);
	m_ctx->gfx_layer_size = m_ctx->xres * m_ctx->yres * fb_var.bits_per_pixel / 8;

	if (m_ctx->framebuffer_size < m_ctx->gfx_layer_size) {
		::close(m_ctx->fd_fb);
		::close(m_ctx->fd_disp);
		delete m_ctx;
		m_ctx = 0;

		qDebug("[%s] /dev/fb0 framebuffer_size error", Q_FUNC_INFO);
		return false;
	}

	/* mmap framebuffer memory */
	m_ctx->framebuffer_addr = (unsigned char *)::mmap(0, m_ctx->framebuffer_size,
											PROT_READ | PROT_WRITE,
											MAP_SHARED, m_ctx->fd_fb, 0);
	if (m_ctx->framebuffer_addr == MAP_FAILED) {
		::close(m_ctx->fd_fb);
		::close(m_ctx->fd_disp);
		delete m_ctx;
		m_ctx = 0;

		qDebug("[%s] /dev/fb0 framebuffer mmap error", Q_FUNC_INFO);
		return false;
	}

		/* Get the id of the screen layer */
	if (::ioctl(m_ctx->fd_fb, FBIOGET_LAYER_HDL_0, &m_ctx->gfx_layer_id))
	{
		::close(m_ctx->fd_fb);
		::close(m_ctx->fd_disp);
		delete m_ctx;
		m_ctx = 0;

		qDebug("[%s] /dev/fb0 layer id error", Q_FUNC_INFO);
		return false;
	}

	qDebug("DBG m_ctx");
	qDebug("xres = %d, y_res = %d, bits_per_pixel = %d", m_ctx->xres, m_ctx->yres, m_ctx->bits_per_pixel);
	qDebug("paddr %p, size %d, height %d", (void *)m_ctx->framebuffer_paddr, m_ctx->framebuffer_size, m_ctx->framebuffer_height);
	qDebug("gfx_layer_size %d, addr %p, gfx_layer_id %d", m_ctx->gfx_layer_size, m_ctx->framebuffer_addr, m_ctx->gfx_layer_id );

	if (layer_reserve() == false )
	{
		::close(m_ctx->fd_fb);
		::close(m_ctx->fd_disp);
		delete m_ctx;
		m_ctx = 0;

		qDebug("[%s] layer reserve error", Q_FUNC_INFO);
		return false;
	}

	return true;
}

void SunxiDisp::end()
{
	if ( m_ctx )
	{
		if (m_ctx->fd_disp >= 0)
		{
			/* release layer */
			layer_release();
			::munmap(m_ctx->framebuffer_addr, m_ctx->framebuffer_size);
			::close(m_ctx->fd_fb);
			::close(m_ctx->fd_disp);
			delete m_ctx;
			m_ctx = 0;

			qDebug("[%s] end", Q_FUNC_INFO);
		}
	}
}

bool SunxiDisp::layer_reserve()
{
	unsigned int tmp[4];

	/* try to allocate a layer */

	tmp[0] = m_ctx->fb_id;
	tmp[1] = DISP_LAYER_WORK_MODE_NORMAL;
	m_ctx->layer_id = ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_REQUEST, &tmp);
	if (m_ctx->layer_id < 0)
	{
		qDebug("[%s] layer request error", Q_FUNC_INFO);
		return false;
	}

	/* Initially set the layer configuration to something reasonable */
	__disp_layer_info_t layer_info;

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	tmp[2] = (unsigned int)&layer_info;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_GET_PARA, tmp) < 0)
	{
		qDebug("[%s] layer get_para error", Q_FUNC_INFO);
		return false;
	}

	/* the screen and overlay layers need to be in different pipes */
	layer_info.pipe      = 1;
	layer_info.alpha_en  = 1;
	layer_info.alpha_val = 255;

	layer_info.fb.addr[0] = m_ctx->framebuffer_paddr;
	layer_info.fb.size.width = 1;
	layer_info.fb.size.height = 1;
	layer_info.fb.format = DISP_FORMAT_ARGB8888;
	layer_info.fb.seq = DISP_SEQ_ARGB;
	layer_info.fb.mode = DISP_MOD_INTERLEAVED;

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	tmp[2] = (unsigned int)&layer_info;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_SET_PARA, tmp) < 0)
	{
		qDebug("[%s] layer set_para error", Q_FUNC_INFO);
		return false;
	}

	qDebug("DBG layer_id = %d", m_ctx->layer_id);

	/* Now probe the scaler mode to see if there is a free scaler available */
	if (change_work_mode(DISP_LAYER_WORK_MODE_SCALER) )
		m_ctx->layer_has_scaler = 1;

	/* Revert back to normal mode */
	change_work_mode(DISP_LAYER_WORK_MODE_NORMAL);
	m_ctx->layer_scaler_is_enabled = 0;
	m_ctx->layer_format = DISP_FORMAT_ARGB8888;

	qDebug("DBG layer_id = %d, has_scalser = %d", m_ctx->layer_id, m_ctx->layer_has_scaler);

	return true;
}

bool SunxiDisp::layer_release()
{
	unsigned int tmp[4];

	if (m_ctx->layer_id < 0)
	{
		qDebug("[%s] layer id error", Q_FUNC_INFO);
		return false;
	}

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_RELEASE, &tmp);

	m_ctx->layer_id = -1;
	m_ctx->layer_has_scaler = 0;
	return true;
}

bool SunxiDisp::change_work_mode(int new_mode)
{
	unsigned int tmp[4];

	if (m_ctx->layer_id < 0)
	{
		qDebug("[%s] layer id error", Q_FUNC_INFO);
		return false;
	}

	__disp_layer_info_t layer_info;

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	tmp[2] = (unsigned int)&layer_info;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_GET_PARA, tmp) < 0)
	{
		qDebug("[%s] layer get_para error", Q_FUNC_INFO);
		return false;
	}
	layer_info.mode = (__disp_layer_work_mode_t)new_mode;

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	tmp[2] = (unsigned int )&layer_info;
	if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_SET_PARA, tmp) < 0 )
	{
		qDebug("[%s] layer set_para error", Q_FUNC_INFO);
		return false;
	}
	return true;
}

bool SunxiDisp::set_rgb_input_buffer(unsigned int offset_in_framebuffer, int width, int height, int stride)
{
	__disp_fb_t fb;
	__disp_rect_t rect = { 0, 0, (unsigned int)width, (unsigned int)height };
	unsigned int tmp[4];
	::memset(&fb, 0, sizeof(fb));

	if (m_ctx->layer_id < 0)
	{
		qDebug("[%s] layer id error", Q_FUNC_INFO);
		return false;
	}

	if (m_ctx->layer_scaler_is_enabled)
	{
		if (change_work_mode(DISP_LAYER_WORK_MODE_NORMAL) )
			m_ctx->layer_scaler_is_enabled = 0;
		else
		{
			qDebug("[%s] change work mode error", Q_FUNC_INFO);
			return false;
		}
	}

	fb.addr[0] = m_ctx->framebuffer_paddr + offset_in_framebuffer;
	fb.size.height = height;
	fb.format = DISP_FORMAT_ARGB8888;
	fb.seq = DISP_SEQ_ARGB;
	fb.mode = DISP_MOD_INTERLEAVED;
	fb.size.width = stride; // width of the buffer in bytes

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	tmp[2] = (unsigned int)&fb;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_SET_FB, &tmp) < 0)
	{
		qDebug("[%s] set_fb error", Q_FUNC_INFO);
		return false;
	}
	m_ctx->layer_buf_x = rect.x;
	m_ctx->layer_buf_y = rect.y;
	m_ctx->layer_buf_w = rect.width;
	m_ctx->layer_buf_h = rect.height;
	m_ctx->layer_format = fb.format;

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	tmp[2] = (unsigned int)&rect;
	if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_SET_SRC_WINDOW, &tmp) < 0 )
	{
		qDebug("[%s] set_src_window error", Q_FUNC_INFO);
		return false;
	}
	return true;
}

bool SunxiDisp::set_output_window(int x, int y, int w, int h)
{
	__disp_rect_t buf_rect = {
		m_ctx->layer_buf_x, m_ctx->layer_buf_y,
		(unsigned int)m_ctx->layer_buf_w, (unsigned int)m_ctx->layer_buf_h
	};
	__disp_rect_t win_rect = { x, y, (unsigned int)w, (unsigned int)h };
	unsigned int tmp[4];

	if (m_ctx->layer_id < 0 || w <= 0 || h <= 0)
	{
		qDebug("[%s] layer id, w, h error", Q_FUNC_INFO);
		return false;
	}

	/*
	* Handle negative window Y coordinates (workaround a bug).
	* The Allwinner A10/A13 display controller hardware is expected to
	* support negative coordinates of the top left corners of the layers.
	* But there is some bug either in the kernel driver or in the hardware,
	* which messes up the picture on screen when the Y coordinate is negative
	* for YUV layer. Negative X coordinates are not affected. RGB formats
	* are not affected too.
	*
	* We fix this by just recalculating which part of the buffer in memory
	* corresponds to Y=0 on screen and adjust the input buffer settings.
	*/
	if (m_ctx->layer_format == DISP_FORMAT_YUV420 &&
	   (y < 0 || m_ctx->layer_win_y < 0))
	{
		if (win_rect.y < 0)
		{
			int y_shift = -(double)y * buf_rect.height / win_rect.height;
			buf_rect.y      += y_shift;
			buf_rect.height -= y_shift;
			win_rect.height += win_rect.y;
			win_rect.y       = 0;
		}

		if (buf_rect.height <= 0 || win_rect.height <= 0)
		{
			/* No part of the window is visible. Just construct a fake rectangle
			* outside the screen as a window placement (but with a non-negative Y
			* coordinate). Do this to avoid passing bogus negative heights to
			* the kernel driver (who knows how it would react?) */
			win_rect.x = -1;
			win_rect.y = 0;
			win_rect.width = 1;
			win_rect.height = 1;
			tmp[0] = m_ctx->fb_id;
			tmp[1] = m_ctx->layer_id;
			tmp[2] = (unsigned int)&win_rect;
			if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_SET_SCN_WINDOW, &tmp) < 0 )
			{
				qDebug("[%s] set_scn_window error", Q_FUNC_INFO);
				return false;
			}
		}

		tmp[0] = m_ctx->fb_id;
		tmp[1] = m_ctx->layer_id;
		tmp[2] = (unsigned int)&buf_rect;
		if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_SET_SRC_WINDOW, &tmp) < 0)
		{
			qDebug("[%s] set_src_window error", Q_FUNC_INFO);
			return false;
		}
	}
	/* Save the new non-adjusted window position */
	m_ctx->layer_win_x = x;
	m_ctx->layer_win_y = y;

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	tmp[2] = (unsigned int)&win_rect;
	if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_SET_SCN_WINDOW, &tmp) < 0 )
	{
		qDebug("[%s] set_scn_window error", Q_FUNC_INFO);
		return false;
	}
	return true;
}


bool SunxiDisp::layer_show()
{
	unsigned int tmp[4];

	if (m_ctx->layer_id < 0)
	{
		qDebug("[%s] layer id error", Q_FUNC_INFO);
		return false;
	}
	/* YUV formats need to use a scaler */
	if (m_ctx->layer_format == DISP_FORMAT_YUV420 && !m_ctx->layer_scaler_is_enabled)
	{
		if ( change_work_mode(DISP_LAYER_WORK_MODE_SCALER) )
			m_ctx->layer_scaler_is_enabled = 1;
	}

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_OPEN, &tmp) < 0 )
	{
		qDebug("[%s] layer open error", Q_FUNC_INFO);
		return false;
	}
	return true;
}

bool SunxiDisp::layer_hide()
{
	unsigned int tmp[4];

	if (m_ctx->layer_id < 0)
	{
		qDebug("[%s] layer id error", Q_FUNC_INFO);
		return false;
	}

	/* If the layer is hidden, there is no need to keep the scaler occupied */
	if (m_ctx->layer_scaler_is_enabled)
	{
		if ( change_work_mode(DISP_LAYER_WORK_MODE_NORMAL) )
			m_ctx->layer_scaler_is_enabled = 0;
	}

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_CLOSE, &tmp) < 0 )
	{
		qDebug("[%s] layer close error", Q_FUNC_INFO);
		return false;
	}
	return true;
}

bool SunxiDisp::set_colorkey(unsigned int color)
{
	unsigned int tmp[4];
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

	tmp[0] = m_ctx->fb_id;
	tmp[1] = (unsigned int)&colorkey;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_SET_COLORKEY, &tmp) < 0 )
	{
		qDebug("[%s] set colorkey error", Q_FUNC_INFO);
		return false;
	}

	/* Set the overlay layer below the screen layer */
	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->gfx_layer_id;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_BOTTOM, &tmp) < 0)
	{
		qDebug("[%s] layer bottom error", Q_FUNC_INFO);
		return false;
	}

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_BOTTOM, &tmp) < 0)
	{
		qDebug("[%s] layer bottom error", Q_FUNC_INFO);
		return false;
	}

	/* Enable color key for the overlay layer */
	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_CK_ON, &tmp) < 0)
	{
		qDebug("[%s] ck on error", Q_FUNC_INFO);
		return false;
	}

	/* Disable color key and enable global alpha for the screen layer */
	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->gfx_layer_id;
	if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_CK_OFF, &tmp) < 0)
	{
		qDebug("[%s] ck off error", Q_FUNC_INFO);
		return false;
	}

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->gfx_layer_id;
	if ( ::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_ALPHA_ON, &tmp) < 0)
	{
		qDebug("[%s] alpha on error", Q_FUNC_INFO);
		return false;
	}
	return true;
}

bool SunxiDisp::disable_colorkey()
{
	unsigned int tmp[4];

	/* Disable color key for the overlay layer */
	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_CK_OFF, &tmp) < 0)
	{
		qDebug("[%s] ck off error", Q_FUNC_INFO);
		return false;
	}

	/* Set the overlay layer above the screen layer */
	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->layer_id;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_BOTTOM, &tmp) < 0)
	{
		qDebug("[%s] layer bottom error", Q_FUNC_INFO);
		return false;
	}

	tmp[0] = m_ctx->fb_id;
	tmp[1] = m_ctx->gfx_layer_id;
	if (::ioctl(m_ctx->fd_disp, DISP_CMD_LAYER_BOTTOM, &tmp) < 0)
	{
		qDebug("[%s] layer bottom error", Q_FUNC_INFO);
		return false;
	}

	return true;
}
