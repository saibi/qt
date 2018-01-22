#ifndef SUNXIDISP_H
#define SUNXIDISP_H

#include <QObject>

#include <linux/fb.h>
#include "sunxi_disp_ioctl.h"

typedef struct {
	int                 fd_fb;
	int                 fd_disp;
	int                 fd_g2d;
	int                 fb_id;             /* /dev/fb0 = 0, /dev/fb1 = 1 */

	int                 xres, yres, bits_per_pixel;
	unsigned char       *framebuffer_addr;  /* mmapped address */
	unsigned long       framebuffer_paddr; /* physical address */
	unsigned int        framebuffer_size;  /* total size of the framebuffer */
	int                 framebuffer_height;/* virtual vertical resolution */
	unsigned int        gfx_layer_size;    /* the size of the primary layer */

	/* Layers support */
	int                 gfx_layer_id;
	int                 layer_id;
	int                 layer_has_scaler;

	int                 layer_buf_x, layer_buf_y, layer_buf_w, layer_buf_h;
	int                 layer_win_x, layer_win_y;
	int                 layer_scaler_is_enabled;
	int                 layer_format;
} sunxi_disp_t;


class SunxiDisp  : public QObject
{
Q_OBJECT


private:
	explicit SunxiDisp(QObject * parent = 0);
	~SunxiDisp();
	Q_DISABLE_COPY(SunxiDisp)


public:
	static SunxiDisp & instance()
	{
		static SunxiDisp sunxiDisp;
		return sunxiDisp;
	}

	bool init();
	void end();

	bool set_rgb_input_buffer(unsigned int offset_in_framebuffer, int width, int height, int stride);
	bool set_output_window(int x, int y, int w, int h);
	bool layer_show();
	bool layer_hide();
	bool set_colorkey(unsigned int color);
	bool disable_colorkey();

protected:
	bool layer_reserve();
	bool layer_release();
	bool change_work_mode(int new_mode);

private:
	sunxi_disp_t * m_ctx;
};

#endif // SUNXIDISP_H
