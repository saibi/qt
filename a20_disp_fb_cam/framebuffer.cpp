#include "framebuffer.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include "sunxi_disp_ioctl.h"

FrameBuffer::FrameBuffer(QObject *parent) : QObject(parent)
{
	m_fdFb = 0;
	m_fbp = 0;
	m_screensize = 0;
	m_virtualSize = 0;
	m_doubleBufferIdx = 0;
	m_fbDepthBytes = 0;
	m_layer = 0;

	open();
}

FrameBuffer::~FrameBuffer()
{
}


bool FrameBuffer::open()
{
	if ( m_fdFb > 0 )
		close();

	// Open the file for reading and writing
	m_fdFb = ::open("/dev/fb0", O_RDWR);
	if (-1 == m_fdFb )
	{
		qDebug("Error: cannot open framebuffer device");
		return false;
	}

	qDebug("The framebuffer device was opened successfully.");

	// Get fixed screen information
	if ( -1 == ::ioctl(m_fdFb, FBIOGET_FSCREENINFO, &m_finfo) )
	{
		qDebug("Error reading fixed information");
		::close(m_fdFb);
		m_fdFb = -2;
		return false;
	}

	// Get variable screen information
	if ( -1 == ::ioctl(m_fdFb, FBIOGET_VSCREENINFO, &m_vinfo) )
	{
		qDebug("Error reading variable information");
		::close(m_fdFb);
		m_fdFb = -3;
		return false;
	}

	qDebug("%dx%d, %dbpp (%d, %d)\n", m_vinfo.xres, m_vinfo.yres, m_vinfo.bits_per_pixel, m_vinfo.xres_virtual, m_vinfo.yres_virtual);

	// Figure out the size of the screen in bytes
	m_fbDepthBytes = m_vinfo.bits_per_pixel / 8;
	m_virtualSize = m_vinfo.xres_virtual * m_vinfo.yres_virtual * m_fbDepthBytes;
	m_screensize = m_vinfo.xres * m_vinfo.yres * m_fbDepthBytes;

	// Map the device to memory
	m_fbp = (char *) ::mmap(0, m_virtualSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fdFb, 0);
	if ((char *)-1 == m_fbp)
	{
		qDebug("Error: failed to map framebuffer device to memory");
		::close(m_fdFb);
		m_fdFb = -4;
		return false;
	}
	qDebug("The framebuffer device was mapped to memory successfully.\n");


	if ( ::ioctl(m_fdFb, FBIOGET_LAYER_HDL_0, &m_layer) == 0 )
	{
		qDebug("layer id = %d", m_layer);
	}

	return true;
}

void FrameBuffer::close()
{
	if ( m_fdFb > 0 && m_fbp)
	{
		::munmap(m_fbp, m_virtualSize);
		::close(m_fdFb);

		m_fdFb = 0;
		m_fbp = 0;
	}
}

void FrameBuffer::clear()
{
	if ( m_fdFb <= 0 )
		return;

	char * framebuffer = m_fbp;
	int location = (0 + m_vinfo.xoffset) * m_fbDepthBytes + (0 + m_vinfo.yoffset) * m_finfo.line_length;
	::memset( framebuffer + location, 0, m_screensize );
}

void FrameBuffer::test(int startx, int starty)
{
	if ( m_fdFb <= 0 )
		return;


	long int location = 0;
	int endx = startx + 200;
	int endy = starty + 200;
	int x, y;

	char * framebuffer = m_fbp;

	// Figure out where in memory to put the pixel
	for (y = starty; y < endy; y++)
		for (x = startx; x < endx; x++)
		{
			location = (x + m_vinfo.xoffset) * m_fbDepthBytes + (y + m_vinfo.yoffset) * m_finfo.line_length;

			if (m_vinfo.bits_per_pixel == 32)
			{
				*(framebuffer + location) = 100;	// Some blue
				*(framebuffer + location + 1) = 15 + (x - 100) / 2;	// A little green
				*(framebuffer + location + 2) = 200 - (y - 100) / 5;	// A lot of red
				*(framebuffer + location + 3) = 0;	// No transparency
			}
			else
			{	//assume 16bpp
				int b = 10;
				int g = (x - 100) / 6;	// A little green
				int r = 31 - (y - 100) / 16;	// A lot of red
				unsigned short int t = r << 11 | g << 5 | b;
				*((unsigned short int *) (m_fbp + location)) = t;
			}

		}
}

// for 32 bpp
void FrameBuffer::fillBox(int x, int y, int w, int h, unsigned int c)
{
	if ( m_fdFb <= 0 )
		return;

	long int location = 0;
	int ex = x + w - 1;
	int ey = y + h - 1;
	int ix, iy;

	char * framebuffer = m_fbp;

	for (iy = y; iy < ey; ++iy)
		for (ix = x; ix < ex; ++ix)
		{
			location = (ix + m_vinfo.xoffset) * m_fbDepthBytes + (iy + m_vinfo.yoffset) * m_finfo.line_length;

			*(framebuffer + location) = c & 0xff; // blue
			*(framebuffer + location + 1) = (c & 0xff00) >> 8; // green
			*(framebuffer + location + 2) = (c & 0xff0000) >> 16; // red
			*(framebuffer + location + 3) = (c & 0xff000000) >> 24; // alpha
		}
}


void FrameBuffer::drawImg(int x, int y, const QImage &img)
{
	if ( m_fdFb <= 0 )
		return;

	QImage tmp = img.convertToFormat(QImage::Format_RGB32);

	int idx;

	int location = 0;

	char * framebuffer = m_fbp;

	for ( idx = 0; idx < img.height() ; ++idx)
	{
		location = (x + m_vinfo.xoffset) * m_fbDepthBytes + (y + idx + m_vinfo.yoffset) * m_finfo.line_length;

		::memcpy( framebuffer + location, tmp.scanLine(idx), img.width() * m_fbDepthBytes );
	}

}

void FrameBuffer::drawCam(int x, int y, unsigned char *camData, int width, int height, int fbIdx)
{
	if ( m_fdFb <= 0 )
		return;

	char *fbData;

	if ( fbIdx < 0 )
		fbData = m_fbp + ( x + m_vinfo.xoffset) * m_fbDepthBytes + ( y + m_vinfo.yoffset) * m_finfo.line_length;
	else
		fbData = m_fbp + ( x + m_vinfo.xoffset) * m_fbDepthBytes + ( y + m_vinfo.yres * fbIdx) * m_finfo.line_length;

	int copyBytes = width * m_fbDepthBytes;

	for ( int idx = 0 ; idx < height ; ++idx )
	{
		if ( (y + idx) >= int(m_vinfo.yres) )
		{
			break;
		}

		memcpy(fbData, camData, copyBytes);
		fbData += m_vinfo.xres * m_fbDepthBytes;
		camData += copyBytes;
	}
}

int FrameBuffer::toggleBuffer()
{
	if ( m_fdFb <= 0 )
		return 0;

	if ( m_doubleBufferIdx == 0 )
	{
		m_doubleBufferIdx = 1;
	}
	else
	{
		m_doubleBufferIdx = 0;
	}

	m_vinfo.yoffset = m_vinfo.yres * m_doubleBufferIdx;


#ifdef FBIO_WAITFORVSYNC
	int dummy = 0;

	if (::ioctl(m_fdFb, FBIO_WAITFORVSYNC, &dummy))
	{
		qDebug("Error - FBIO_WAITFORVSYNC");
		return 0;
	}
#endif

	if (::ioctl(m_fdFb, FBIOPAN_DISPLAY, &m_vinfo))
	{
		qDebug("Error - FBIOPAN_DISPLAY");
		return 0;
	}


	return m_doubleBufferIdx;
}

void FrameBuffer::copy(int idx)
{
	if ( idx == 0 )
		::memcpy(m_fbp + m_screensize, m_fbp, m_screensize);
	else
		::memcpy(m_fbp, m_fbp + m_screensize, m_screensize);
}
