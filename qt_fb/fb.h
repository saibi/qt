#ifndef _FB_H_
#define _FB_H_

#include <QObject>
#include <linux/fb.h>
#include <QImage>


class FrameBuffer : public QObject
{
Q_OBJECT

private:
	explicit FrameBuffer(QObject * parent = 0);
	~FrameBuffer();
	Q_DISABLE_COPY(FrameBuffer)

public:
	static FrameBuffer & instance()
	{
		static FrameBuffer fb;
		return fb;
	}

	bool open();
	void close();

	void test(int startx = 100, int starty = 100);
	void drawImg(int x, int y, const QImage & img);
	int toggleBuffer();
	void copy(int idx);

private:
	int m_fbfd;
	char * m_fbp;
	int m_screensize;
	int m_virtualSize;
	int m_fbDepthBytes;

	struct fb_var_screeninfo m_vinfo;
	struct fb_fix_screeninfo m_finfo;
	int m_doubleBufferIdx;

};

#if 0
int fb_test(int startx, int starty);
#endif


#endif

