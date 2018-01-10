#include "fb.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

FrameBuffer::FrameBuffer(QObject *parent) : QObject(parent)
{
	m_fbfd = -1;
	m_fbp = 0;
	m_screensize = 0;
	m_virtualSize = 0;
	m_doubleBufferIdx = 0;
	m_fbDepthBytes = 0;

	open();
}

FrameBuffer::~FrameBuffer()
{
}


bool FrameBuffer::open()
{
	if ( m_fbfd >= 0 )
		close();

	// Open the file for reading and writing
	m_fbfd = ::open("/dev/fb0", O_RDWR);
	if (-1 == m_fbfd )
	{
		qDebug("Error: cannot open framebuffer device");
		return false;
	}

	qDebug("The framebuffer device was opened successfully.");

	// Get fixed screen information
	if ( -1 == ::ioctl(m_fbfd, FBIOGET_FSCREENINFO, &m_finfo) )
	{
		qDebug("Error reading fixed information");
		::close(m_fbfd);
		m_fbfd = -2;
		return false;
	}

	// Get variable screen information
	if ( -1 == ::ioctl(m_fbfd, FBIOGET_VSCREENINFO, &m_vinfo) )
	{
		qDebug("Error reading variable information");
		::close(m_fbfd);
		m_fbfd = -3;
		return false;
	}

	qDebug("%dx%d, %dbpp (%d, %d)\n", m_vinfo.xres, m_vinfo.yres, m_vinfo.bits_per_pixel, m_vinfo.xres_virtual, m_vinfo.yres_virtual);

	// Figure out the size of the screen in bytes
	m_fbDepthBytes = m_vinfo.bits_per_pixel / 8;
	m_virtualSize = m_vinfo.xres_virtual * m_vinfo.yres_virtual * m_fbDepthBytes;
	m_screensize = m_vinfo.xres * m_vinfo.yres * m_fbDepthBytes;

	// Map the device to memory
	m_fbp = (char *) ::mmap(0, m_virtualSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fbfd, 0);
	if ((char *)-1 == m_fbp)
	{
		qDebug("Error: failed to map framebuffer device to memory");
		::close(m_fbfd);
		m_fbfd = -4;
		return false;
	}
	qDebug("The framebuffer device was mapped to memory successfully.\n");
	return true;

}

void FrameBuffer::close()
{
	if ( m_fbfd >= 0 && m_fbp)
	{
		::munmap(m_fbp, m_virtualSize);
		::close(m_fbfd);

		m_fbfd = -1;
		m_fbp = 0;
	}
}

void FrameBuffer::test(int startx, int starty)
{
	if ( m_fbfd < 0 )
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



	struct v4l2_framebuffer p;

	::memset(&p, 0, sizeof(p));

	if ( ::ioctl(m_fbfd, VIDIOC_G_FBUF, &p) )
	{
		qDebug("Error - VIDIOC_G_FBUF");
	}
	else
	{
		qDebug("VIDIOC_G_FBUF");
		qDebug("  capability 0x%x", p.capability);
		qDebug("  flags 0x%x", p.flags);
		qDebug("  base %p", p.base);
		qDebug("  width %d", p.fmt.width);
		qDebug("  height %d", p.fmt.height);
		qDebug("  pixelformat 0x%x", p.fmt.pixelformat);
		qDebug("  field 0x%x", p.fmt.field);
		qDebug("  bytesperline %d", p.fmt.bytesperline);
		qDebug("  sizeimage %d", p.fmt.sizeimage);
		qDebug("  colorspace 0x%x", p.fmt.colorspace);
		qDebug("  priv 0x%x", p.fmt.priv);

	}
}

void FrameBuffer::drawImg(int x, int y, const QImage &img)
{
	if ( m_fbfd < 0 )
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

int FrameBuffer::toggleBuffer()
{
	if ( m_fbfd < 0 )
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

	if (::ioctl(m_fbfd, FBIO_WAITFORVSYNC, &dummy))
	{
		qDebug("Error - FBIO_WAITFORVSYNC");
		return 0;
	}
#endif

	if (::ioctl(m_fbfd, FBIOPAN_DISPLAY, &m_vinfo))
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


#if 0

int fb_test(int startx, int starty)
{
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;
	int x = 0, y = 0;
	long int location = 0;

	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1)
	{
		perror("Error: cannot open framebuffer device");
		return -1;
	}
	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1)
	{
		perror("Error reading fixed information");
		close(fbfd);
		return -2;
	}
	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1)
	{
		perror("Error reading variable information");
		close(fbfd);
		return -3;
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int) fbp == -1)
	{
		perror("Error: failed to map framebuffer device to memory");
		close(fbfd);
		return -4;
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

	int endx = startx + 200;
	int endy = starty + 200;	// Where we are going to put the pixel

	// Figure out where in memory to put the pixel
	for (y = starty; y < endy; y++)
		for (x = startx; x < endx; x++)
		{

			location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * finfo.line_length;

			if (vinfo.bits_per_pixel == 32)
			{
				*(fbp + location) = 100;	// Some blue
				*(fbp + location + 1) = 15 + (x - 100) / 2;	// A little green
				*(fbp + location + 2) = 200 - (y - 100) / 5;	// A lot of red
				*(fbp + location + 3) = 0;	// No transparency
			}
			else
			{	//assume 16bpp
				int b = 10;
				int g = (x - 100) / 6;	// A little green
				int r = 31 - (y - 100) / 16;	// A lot of red
				unsigned short int t = r << 11 | g << 5 | b;
				*((unsigned short int *) (fbp + location)) = t;
			}

		}


	munmap(fbp, screensize);
	close(fbfd);
	return 0 ;
}
#endif
