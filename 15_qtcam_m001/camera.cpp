/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 *
 *	This program were got from V4L2 API, Draft 0.20
 *		available at: http://v4l2spec.bytesex.org/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>
#include <linux/fb.h>

//#include <QApplication>
//#include "imageViewer.h"
#include <thread.h>

#define VIDEODEV 		"/dev/video0"
#define FBDEVFILE       "/dev/fb0"
//#define FORCED_WIDTH  	800
//#define FORCED_HEIGHT 	480
//#define FORCED_FORMAT 	V4L2_PIX_FMT_YUYV
//#define FORCED_FORMAT 	V4L2_PIX_FMT_YUV422P
//#define FORCED_FORMAT 	V4L2_PIX_FMT_NV12
#define FORCED_FORMAT 	V4L2_PIX_FMT_GREY
//#define FORCED_FIELD  	V4L2_FIELD_ANY
#define FORCED_FIELD  	V4L2_FIELD_NONE
#define MAX_CAM_BUFFER	4
static int verbose = 1;
#define pr_debug(fmt, arg...) \
	if (verbose) fprintf(stderr, fmt, ##arg)

#define CLEAR(x) memset(&(x), 0, sizeof(x))

enum io_method {
	IO_METHOD_READ,
	IO_METHOD_MMAP,
	IO_METHOD_USERPTR,
};
/*
struct buffer {
	void   *start;
	size_t  length;
};
*/
static char            *dev_name;
static enum io_method   io = IO_METHOD_MMAP;
//static enum io_method   io = IO_METHOD_READ;
static int              fd = -1;
struct buffer          *buffers;
static unsigned int     n_buffers;
static int				out_buf;
struct v4l2_buffer 		buf[MAX_CAM_BUFFER];
//1 => to update new parameter
//0 => to reserve old parameter 
static int              force_format=1;
//static int              frame_count = 40;
//unsigned int 			*pBmpData;
unsigned int 			*pfbmap;
unsigned int 			 pixel;
//extern  uchar      		*pRGBData;
int 					cols,rows,fbfd;
int 					width = FORCED_WIDTH;
int 					height = FORCED_HEIGHT;
int 				led_fd;

struct fb_var_screeninfo vinfo;
//QApplication* app;
//QLabel* label;

//ImageViewer* imageViewer;
//MyThread myThread;

void open_device(void);
void init_device(void);
void start_capturing(void);
int  mainloop(void);
void stop_capturing(void);
void uninit_device(void);
void close_device(void);
void g_led_on(void);
void g_led_off(void);

void g_led_on(void)
{
	int r;
  	r = write(led_fd,"0",1);
}
void g_led_off(void)
{
	int r;
  	r = write(led_fd,"1",1);
}
unsigned int  makepixel(unsigned char r, unsigned char g, unsigned char b) {
    return (unsigned int)((r<<16)|(g<<8)|(b<<0));
}

static void errno_exit(const char *s)
{
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
	exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
	int r;

	do {
		r = ioctl(fh, request, arg);
	} while (-1 == r && EINTR == errno);

	return r;
}


//---------------------------------------------------------
//   getControl
//---------------------------------------------------------

int getControl(int control , int *value)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control_s;
    int err;

    //queryctrl.id = control;
    //if (ioctl(fd, VIDIOC_QUERYCTRL, queryctrl) < 0) return -1;

    control_s.id = control;
    if ((err = ioctl(fd, VIDIOC_G_CTRL, &control_s)) < 0) {
        pr_debug("ioctl get control error\n");
        return -1;
        }
    *value = control_s.value;
    return 0;
}




//---------------------------------------------------------
//   setControl
//---------------------------------------------------------

int setControl(int control, int value)
{
    struct v4l2_control control_s;
    struct v4l2_queryctrl queryctrl;
    int min, max;
    int err;

    queryctrl.id = control;
    if (ioctl(fd, VIDIOC_QUERYCTRL, queryctrl) < 0) return -1;

    min = queryctrl.minimum;
    max = queryctrl.maximum;
    if ((value >= min) && (value <= max)) {
    control_s.id = control;
    control_s.value = value;
    if ((err = ioctl(fd, VIDIOC_S_CTRL, &control_s)) < 0) {
          pr_debug("ioctl set control error\n");
          return -1;
          }
    }
    return 0;
}


//---------------------------------------------------------
//   upControl
//---------------------------------------------------------

int upControl(int control)
{
    struct v4l2_control control_s;
    struct v4l2_queryctrl queryctrl;
    int max, current, step;
    int err;

    queryctrl.id = control;
    if (ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl) < 0)
	{
        pr_debug("ioctl query cotrol error\n");
		return -1;
	}
    max  = queryctrl.maximum;
    step = queryctrl.step;
    getControl(control , &current);
    //current = getControl(control);
    current += step;
    if (current <= max) {
        control_s.id = control;
        control_s.value = current;
        if ((err = ioctl(fd, VIDIOC_S_CTRL, &control_s)) < 0) {
            pr_debug("ioctl set control error\n");
            return -1;
        }
        pr_debug("Control name:%s set to value:%d\n", queryctrl.name, control_s.value);
    }
    else {
        pr_debug("Control name:%s already has max value:%d \n", queryctrl.name, max);
    }
    return 0; //control_s.value;
}

//---------------------------------------------------------
//   downControl
//---------------------------------------------------------

int downControl(int control)
{
    struct v4l2_control control_s;
    struct v4l2_queryctrl queryctrl;
    int min, current, step;
    int err;

    queryctrl.id = control;
    if (ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl) < 0)
	{
        pr_debug("ioctl query cotrol error\n");
		return -1;
	}

    min = queryctrl.minimum;
    step = queryctrl.step;
    getControl(control , &current);
    current -= step;
    if (current >= min) {
        control_s.id = control;
        control_s.value = current;
        if ((err = ioctl(fd, VIDIOC_S_CTRL, &control_s)) < 0) {
              pr_debug("ioctl set control error\n");
              return -1;
              }
        pr_debug("Control name:%s set to value:%d\n", queryctrl.name, control_s.value);
        }
    else
        pr_debug("Control name:%s already has min value:%d \n", queryctrl.name, min);
    return 0; //control_s.value;
}

void upControl_gain(void)
{
	upControl(V4L2_CID_GAIN);
}
void downControl_gain(void)
{
	downControl(V4L2_CID_GAIN);
}
void upControl_exp(void)
{
	upControl(V4L2_CID_EXPOSURE);
}
void downControl_exp(void)
{
	downControl(V4L2_CID_EXPOSURE);
}
void upControl_rowStart(void)
{
	upControl(V4L2_CID_AUDIO_BASS);
}
void downControl_rowStart(void)
{
	downControl(V4L2_CID_AUDIO_BASS);
}
void upControl_colStart(void)
{
	upControl(V4L2_CID_AUDIO_TREBLE);
}
void downControl_colStart(void)
{
	downControl(V4L2_CID_AUDIO_TREBLE);
}



/*
inline int clip(int value, int min, int max)
{
    return(value > max ? max : value < min ? min : value);
}

*/
static void process_image(const void *p, int size,int index_buf)
{
	unsigned char *in = (unsigned char *)p;
    //unsigned int pixel;
   // int istride = FORCED_WIDTH*2;          
    //int istride = FORCED_WIDTH;          
  //  int x, y, j;
  //  int y0, u, y1, v, r, g, b;
  //  long location = 0;
  //  long position = 0;

	//pr_debug("%s: called!\n", __func__);



//	if (out_buf)
//		fwrite(p, size, 1, stdout);

//	fflush(stderr);
//	fprintf(stderr,"\nIndex=%d addr=0x%08x | ",index_buf,(unsigned char *)in);
	fprintf(stderr,"\nIndex=%d | ",index_buf);
	fprintf(stderr, "%02x %02x %02x %02x ",in[0],in[1],in[2],in[3]);
	fprintf(stderr, "%02x %02x %02x %02x ",in[4],in[5],in[6],in[7]);
//	fprintf(stderr, "%02x %02x %02x %02x ",in[8],in[9],in[10],in[11]);
//	fprintf(stderr, "%02x %02x %02x %02x ",in[12],in[13],in[14],in[15]);
//	fflush(stdout);
#if 0
    // 이미지의 폭을 넘어가면 다음 라인으로 내려가도록 설정 
    for(y = 0; y < height; ++y) {
        //for(j = 0, x = 0; j < (int)vinfo.xres * 2; j += 4, x += 2) {
        for(j = 0, x = 0; j < istride; j += 4, x += 2) {
			// 현재의 화면에서 이미지를 넘어서는 빈 공간을 처리
           // if(j >= width*2) {       
           //      location++; location++;
           //      continue;
          //  }
            // YUYV 성분을 분리 
            y0 = in[j];
            u = in[j + 1] - 128;
            y1 = in[j + 2];
            v = in[j + 3] - 128;

            // YUV를 RGB로 전환 
            r = clip((298 * y0 + 409 * v + 128) >> 8, 0, 255);
            g = clip((298 * y0 - 100 * u - 208 * v + 128) >> 8, 0, 255);
            b = clip((298 * y0 + 516 * u + 128) >> 8, 0, 255);
			
			pRGBData[position*3+0]= r;
			pRGBData[position*3+1]= g;
			pRGBData[position*3+2]= b;
			position++;

//            pixel =makepixel(r,g,b);    //32bit
//            pBmpData[location++] = pixel;

            // YUV를 RGB로 전환 
            r = clip((298 * y1 + 409 * v + 128) >> 8, 0, 255);
            g = clip((298 * y1 - 100 * u - 208 * v + 128) >> 8, 0, 255);
            b = clip((298 * y1 + 516 * u + 128) >> 8, 0, 255);


			pRGBData[position*3+0]= r;
			pRGBData[position*3+1]= g;
			pRGBData[position*3+2]= b;
			position++;

//            pixel =makepixel(r,g,b);    //32bit
//            pBmpData[location++] = pixel;
        };
        in += istride;
    };
//	memcpy(pfbmap, pBmpData, cols*rows*2);
//	printf("Image Display Start\r\n");
//	label->setText("Camera Image Processing");
//label->setPixmap(QPixmap::fromImage(QImage((uchar *)pRGBData,
//	imageViewer->vw_width,imageViewer->vw_height,QImage::Format_RGB888)));
//     label->show();

//printf("Image Display End\r\n");
#endif
}

static int read_frame(void)
{
	struct v4l2_buffer buff;
	enum v4l2_buf_type type;
	unsigned int i;

//	pr_debug("%s: called!\n", __func__);
	//pr_debug("\r\nRead Data = ");

	switch (io) {
	case IO_METHOD_READ:
		if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
			switch (errno) {
			case EAGAIN:
				return 0;

			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */
			default:
				errno_exit("read");
			}
		}

		process_image(buffers[0].start, buffers[0].length,0);
		break;

	case IO_METHOD_MMAP:

		CLEAR(buff);


		buff.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buff.memory = V4L2_MEMORY_MMAP;
  		//if (-1 == xioctl(fd, VIDIOC_QBUF, &buff))
		//	errno_exit("VIDIOC_QBUF");
		//pr_debug("END QBUF\n");

		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buff)) {
			switch (errno) {
			case EAGAIN:
				printf("EAGAIN ");
				return 0;

			case EIO:
				printf("EIO ");
				/* Could ignore EIO, see spec. */
				/* fall through */

			default:
				errno_exit("VIDIOC_DQBUF_RF");
			}
		}
//		pr_debug("END DQBUF\n");

//		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
//			errno_exit("VIDIOC_QBUF55");

	//	if (-1 == xioctl(fd, VIDIOC_DQBUF, &buff))
	//		errno_exit("VIDIOC_DQBUF22");

		if (-1 == xioctl(fd, VIDIOC_QBUF, &buff))
			errno_exit("VIDIOC_QBUF_RF");

		return buff.index;
		//assert(buff.index < n_buffers);
		//printf("rr_buf.byteuse=%d ",buf.bytesused);
		//if(buf.index == n_buffers)
		//printf("buf_addr=0x%08x ",buffers[buff.index].start);
		//fflush(stdout);
		//process_image(buffers[buff.index].start, buff.bytesused,buff.index);
		//printf("rr_buf.index=%d ",buf.index);

  		//if (-1 == xioctl(fd, VIDIOC_QBUF, &buff))
		//	errno_exit("VIDIOC_QBUF");
		break;

	case IO_METHOD_USERPTR:
		CLEAR(buf);

		buff.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buff.memory = V4L2_MEMORY_USERPTR;

		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buff)) {
			switch (errno) {
			case EAGAIN:
				return 0;

			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */

			default:
				errno_exit("VIDIOC_DQBUF");
			}
		}

		for (i = 0; i < n_buffers; ++i)
			if (buff.m.userptr == (unsigned long)buffers[i].start
			    && buff.length == buffers[i].length)
				break;

		assert(i < n_buffers);

		process_image((void *)buff.m.userptr, buff.bytesused,i);

		if (-1 == xioctl(fd, VIDIOC_QBUF, &buff))
			errno_exit("VIDIOC_QBUF");
		break;
	}

	return 1;
}

int mainloop(void)
{
	//unsigned int count;
	int i;	
//	pr_debug("%s: called!\n", __func__);

//	count = frame_count;

//	while (count-- > 0) {
//		for (;;) 
		{
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(fd, &fds);

			/* Timeout. */
			tv.tv_sec = 4;
			tv.tv_usec = 0;

			r = select(fd + 1, &fds, NULL, NULL, &tv);

			if (-1 == r) {
				//if (EINTR == errno)
				//	continue;
				errno_exit("select");
			}

			else if (0 == r) {
				fprintf(stderr, "select timeout\n");
				exit(EXIT_FAILURE);
			}
/*
			else
			{
				if(!FD_ISSET(fd, &fds))
				{
					continue;
				}
			}
*/
//			fprintf(stderr, "start Read Frame\n");
			 return read_frame();
/*			if (read_frame())

				break;
*/
			/* EAGAIN - continue select loop. */
		}
//	}
}

void stop_capturing(void)
{
	enum v4l2_buf_type type;

	pr_debug("%s: called!\n", __func__);
	switch (io) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;

	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
			errno_exit("VIDIOC_STREAMOFF");
		break;
	}
}

void start_capturing(void)
{
	unsigned int i;
	enum v4l2_buf_type type;
	int err;

	n_buffers = MAX_CAM_BUFFER;
//	pr_debug("%s: called!\n", __func__);
//	pr_debug("\tn_buffers: %d\n", n_buffers);

	switch (io) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;

	case IO_METHOD_MMAP:
//		pr_debug("Before STREAMON\n");
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
			errno_exit("VIDIOC_STREAMON");
//		pr_debug("After STREAMON\n");
		break;

	case IO_METHOD_USERPTR:

/*		for (i = 0; i < n_buffers; ++i) {
			struct v4l2_buffer buf;

			CLEAR(buf);
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_USERPTR;
			buf.index = i;
			buf.m.userptr = (unsigned long)buffers[i].start;
			buf.length = buffers[i].length;

			if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
				errno_exit("VIDIOC_QBUF");
		}*/
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
			errno_exit("VIDIOC_STREAMON");
		break;
	}
}

void uninit_device(void)
{
	unsigned int i;

	pr_debug("%s: called!\n", __func__);

	switch (io) {
	case IO_METHOD_READ:
		free(buffers[0].start);
		break;

	case IO_METHOD_MMAP:
		for (i = 0; i < n_buffers; ++i)
			if (-1 == munmap(buffers[i].start, buffers[i].length))
				errno_exit("munmap");
		break;

	case IO_METHOD_USERPTR:
		for (i = 0; i < n_buffers; ++i)
			free(buffers[i].start);
		break;
	}

	free(buffers);
	munmap(pfbmap, cols*rows*2);
//    free(pBmpData);
   // free(pRGBData);
    //free(pData);
    close(fbfd);
}

static void init_read(unsigned int buffer_size)
{
	pr_debug("%s: called!\n", __func__);

	buffers = (buffer *)calloc(1, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	buffers[0].length = buffer_size;
	buffers[0].start = malloc(buffer_size);

	if (!buffers[0].start) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
}

static void init_mmap(void)
{
	struct v4l2_requestbuffers req;
	int i_buffers=0;

	//pr_debug("%s: called!\n", __func__);

	/////////Init Frame Buffer/////
	/* 프레임버퍼를 오픈한다. */
	pr_debug("Init_mmap\r\n");
/*
	pr_debug("FB Init Start\r\n");

    fbfd = open(FBDEVFILE, O_RDWR);
    if(fbfd < 0) {
        perror("open( )");
		exit(EXIT_FAILURE);
    }

    // 현재 프레임버퍼에 대한 고정된 화면 정보를 얻어온다. 
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("ioctl( ) : FBIOGET_VSCREENINFO");
		exit(EXIT_FAILURE);
    }

    cols = vinfo.xres;
    rows = vinfo.yres;

    // BMP 출력을 위한 변수의 메모리 할당 
    //pBmpData = (unsigned int *)malloc(cols * rows * sizeof(unsigned int));
    //pRGBData = (uchar *)malloc(width * height * 3 * sizeof(uchar));

    // 프레임버퍼에 대한 메모리 맵을 수행한다. 
    pfbmap = (unsigned int *)mmap(0, cols*rows*2, PROT_READ|PROT_WRITE,  MAP_SHARED, fbfd, 0);
        if((unsigned)pfbmap == (unsigned)-1) {
        perror("mmap( )");
		exit(EXIT_FAILURE);
    }

    pr_debug("FB Init ENd\r\n");
*/
	CLEAR(req);

	req.count = MAX_CAM_BUFFER ;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	pr_debug("Init_mmap req.count=%d \r\n",req.count);
	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
				 "memory mapping\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}
//	pr_debug("\treq.count: %d\n", req.count);
//	pr_debug("\treq.type: %d\n", req.type);
//	pr_debug("\treq.memory: %d\n", req.memory);
//	pr_debug("\n");


	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n",
			 dev_name);
		exit(EXIT_FAILURE);
	}

	buffers = (buffer *)calloc(req.count, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (i_buffers = 0; i_buffers < (int)req.count;i_buffers++ ) 
	{

		//struct v4l2_buffer buf;
		req.count = MAX_CAM_BUFFER ;

		CLEAR(buf[i_buffers]);

		buf[i_buffers].type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf[i_buffers].memory      = V4L2_MEMORY_MMAP;
		buf[i_buffers].index       = i_buffers;
		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf[i_buffers]))
			errno_exit("VIDIOC_QBUF");

//		pr_debug("\tbuf.index: %d\n", buf.index);
//		pr_debug("\tbuf.type: %d\n", buf.type);
//		pr_debug("\tbuf.bytesused: %d\n", buf.bytesused);
//		pr_debug("\tbuf.flags: %d\n", buf.flags);
//		pr_debug("\tbuf.field: %d\n", buf.field);
//		pr_debug("\tbuf.timestamp.tv_sec: %ld\n", (long) buf.timestamp.tv_sec);
//		pr_debug("\tbuf.timestamp.tv_usec: %ld\n", (long) buf.timestamp.tv_usec);
//		pr_debug("\tbuf.timecode.type: %d\n", buf.timecode.type);
//		pr_debug("\tbuf.timecode.flags: %d\n", buf.timecode.flags);
//		pr_debug("\tbuf.timecode.frames: %d\n", buf.timecode.frames);
//		pr_debug("\tbuf.timecode.seconds: %d\n", buf.timecode.seconds);
//		pr_debug("\tbuf.timecode.minutes: %d\n", buf.timecode.minutes);
//		pr_debug("\tbuf.timecode.hours: %d\n", buf.timecode.hours);
//		pr_debug("\tbuf.timecode.userbits: %d,%d,%d,%d\n",
//				buf.timecode.userbits[0],
//				buf.timecode.userbits[1],
//				buf.timecode.userbits[2],
//				buf.timecode.userbits[3]);
//		pr_debug("\tbuf.sequence: %d\n", buf.sequence);
//		pr_debug("\tbuf.memory: %d\n", buf.memory);
		pr_debug("\tbuf.m.offset: %d\n", buf[i_buffers].m.offset);
		pr_debug("\tbuf.length: %d\n", buf[i_buffers].length);
		pr_debug("\ti_buffers =%d req.count=%d \n", i_buffers, req.count);
		//pr_debug("\tbuf.input: %d\n", buf.input);
//		pr_debug("\n");

		buffers[i_buffers].length = buf[i_buffers].length;
		buffers[i_buffers].start =
			mmap(NULL /* start anywhere */,
			      buf[i_buffers].length,
			      PROT_READ | PROT_WRITE /* required */,
			      MAP_SHARED /* recommended */,
			      fd, buf[i_buffers].m.offset);

		pr_debug("buf Index = %d start=0x%08x\n",i_buffers,buffers[i_buffers].start);
		if (MAP_FAILED == buffers[i_buffers].start)
			errno_exit("mmap");
//		pr_debug("buf mmap OK n_Buffer=%d\n",n_buffers);
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf[i_buffers]))
			errno_exit("VIDIOC_QBUF");
	}
/*
	for (i_buffers = 0; i_buffers < (req.count-0);i_buffers++ ) 
	{
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf[i_buffers]))
			errno_exit("VIDIOC_QBUF");
	}
*/
		pr_debug("END Init MMap\n");

}

static void init_userp(unsigned int buffer_size)
{
	struct v4l2_requestbuffers req;

	//pr_debug("%s: called!\n", __func__);
	pr_debug("init_userp\n");

	CLEAR(req);

	req.count  = MAX_CAM_BUFFER ;
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
				 "user pointer i/o\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}

	buffers = (buffer *)calloc(4, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < MAX_CAM_BUFFER; ++n_buffers) {
		buffers[n_buffers].length = buffer_size;
		buffers[n_buffers].start = malloc(buffer_size);

		if (!buffers[n_buffers].start) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}
	}
	
			fprintf(stderr, "LAST n_buffers=%d\n",n_buffers);
}

void init_device(void)
{
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	struct v4l2_control control_s;
	struct v4l2_standard standard;
	unsigned int min;
	int i;

	pr_debug("%s: called!\n", __func__);

	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n",
				 dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_QUERYCAP");
		}
	}

	pr_debug("\tdriver: %s\n"
		 "\tcard: %s \n"
		 "\tbus_info: %s\n",
			cap.driver, cap.card, cap.bus_info);
	pr_debug("\tversion: %u.%u.%u\n",
			(cap.version >> 16) & 0xFF,
			(cap.version >> 8) & 0xFF,
			cap.version & 0xFF);
	pr_debug("\tcapabilities: 0x%08x\n", cap.capabilities);

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is no video capture device\n",
			 dev_name);
		exit(EXIT_FAILURE);
	}

	switch (io) {
	case IO_METHOD_READ:
		if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
			fprintf(stderr, "%s does not support read i/o\n",
				 dev_name);
			exit(EXIT_FAILURE);
		}
		break;

	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
			fprintf(stderr, "%s does not support streaming i/o\n",
				 dev_name);
			exit(EXIT_FAILURE);
		}
		break;
	}
	/* Select video input, video standard and tune here. */

	CLEAR(cropcap);

	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
		pr_debug("\tcropcap.type: %d\n", cropcap.type);
		pr_debug("\tcropcap.bounds.left: %d\n", cropcap.bounds.left);
		pr_debug("\tcropcap.bounds.top: %d\n", cropcap.bounds.top);
		pr_debug("\tcropcap.bounds.width: %d\n", cropcap.bounds.width);
		pr_debug("\tcropcap.bounds.height: %d\n", cropcap.bounds.height);

//		pr_debug("\tcropcap.defrect.left: %d\n", cropcap.defrect.left);
//		pr_debug("\tcropcap.defrect.top: %d\n", cropcap.defrect.top);
//		pr_debug("\tcropcap.defrect.width: %d\n", cropcap.defrect.width);
//		pr_debug("\tcropcap.defrect.height: %d\n", cropcap.defrect.height);

		pr_debug("\tcropcap.pixelaspect.numerator: %d\n", cropcap.pixelaspect.numerator);
		pr_debug("\tcropcap.pixelaspect.denominator: %d\n", cropcap.pixelaspect.denominator);
		pr_debug("\n");
		
		CLEAR(crop);
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect; /* reset to default */

		if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
			case EINVAL:
				/* Cropping not supported. */
				break;
			default:
				/* Errors ignored. */
				pr_debug("\tcropping not supported\n");
				break;
			}
		}
	} else {
		/* Errors ignored. */
	}

	for(i =0;; i++)
	{
		struct v4l2_fmtdesc fmtdesc;
		fmtdesc.index = i;
		fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
			break;
		pr_debug("\tSUPPORT FORMAT: %c,%c,%c,%c\n",
				fmtdesc.pixelformat & 0xFF,
				(fmtdesc.pixelformat >> 8) & 0xFF,
				(fmtdesc.pixelformat >> 16) & 0xFF,
				(fmtdesc.pixelformat >> 24) & 0xFF
				);
		//pr_debug("Format  %s\n",fmtdesc.pixelformat);
	}		

	CLEAR(fmt);
	
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (force_format) {
		fmt.fmt.pix.width       = FORCED_WIDTH;
		fmt.fmt.pix.height      = FORCED_HEIGHT;
		fmt.fmt.pix.pixelformat = FORCED_FORMAT;
		fmt.fmt.pix.field       = FORCED_FIELD;

		pr_debug("\tfmt.fmt.pix.pixelformat: %c,%c,%c,%c\n",
				fmt.fmt.pix.pixelformat & 0xFF,
				(fmt.fmt.pix.pixelformat >> 8) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 16) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 24) & 0xFF
				);
		pr_debug("\n");

		pr_debug("\tVIDOC_S_FMT Start\n");
		if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
			errno_exit("VIDIOC_S_FMT");

		/* Note VIDIOC_S_FMT may change width and height. */
	} else {
		/* Preserve original settings as set by v4l2-ctl for example */
		if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
			errno_exit("VIDIOC_G_FMT");

		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		pr_debug("\tfmt.fmt.pix.pixelformat: %c,%c,%c,%c\n",
				fmt.fmt.pix.pixelformat & 0xFF,
				(fmt.fmt.pix.pixelformat >> 8) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 16) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 24) & 0xFF
				);
	}


 	setControl(V4L2_CID_GAIN,20);

	/* Buggy driver paranoia. */
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;

	switch (io) {
	case IO_METHOD_READ:
		init_read(fmt.fmt.pix.sizeimage);
		break;

	case IO_METHOD_MMAP:
		init_mmap();
		break;

	case IO_METHOD_USERPTR:
		init_userp(fmt.fmt.pix.sizeimage);
		break;
	}
}

void close_device(void)
{
	pr_debug("%s: called!\n", __func__);

	if (-1 == close(fd))
		errno_exit("close");
	fd = -1;

	g_led_off();
	close(led_fd);

}

void open_device(void)
{
	struct stat st;
	
	dev_name = (char *) VIDEODEV;

	pr_debug("%s: called!\n", __func__);

	if (-1 == stat(dev_name, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n",
			 dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", dev_name);
		exit(EXIT_FAILURE);
	}

	fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

	if (-1 == fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n",
			 dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	led_fd = open("/sys/class/leds/green_ph20_led1/brightness",O_WRONLY);
    if(led_fd < 0) qDebug() << "led Open Error";

}
/*
static void usage(FILE *fp, int argc, char **argv)
{
	if(argc > 1)
    fprintf(fp,
		 "Usage: %s [options]\n\n"
		 "Version 1.3\n"
		 "Options:\n"
		 "-d | --device name   Video device name [%s]\n"
		 "-h | --help          Print this message\n"
		 "-m | --mmap          Use memory mapped buffers [default]\n"
		 "-r | --read          Use read() calls\n"
		 "-u | --userp         Use application allocated buffers\n"
		 "-o | --output        Outputs stream to stdout\n"
		 "-f | --format        Force format to 640x480 YUYV\n"
		 "-c | --count         Number of frames to grab [%i]\n"
		 "-v | --verbose       Verbose output\n"
		 "",
		 argv[0], dev_name, frame_count);
}
*/
static const char short_options[] = "d:hmruofc:v";

static const struct option
long_options[] = {
	{ "device", required_argument, NULL, 'd' },
	{ "help",   no_argument,       NULL, 'h' },
	{ "mmap",   no_argument,       NULL, 'm' },
	{ "read",   no_argument,       NULL, 'r' },
	{ "userp",  no_argument,       NULL, 'u' },
	{ "output", no_argument,       NULL, 'o' },
	{ "format", no_argument,       NULL, 'f' },
	{ "count",  required_argument, NULL, 'c' },
	{ "verbose", no_argument,      NULL, 'v' },
	{ 0, 0, 0, 0 }
};

#if 0
int main(int argc, char **argv)
{
	dev_name = (char *) VIDEODEV;
	for (;;) {
		int idx;
		int c;

		c = getopt_long(argc, argv, short_options, long_options, &idx);

		if (-1 == c)
			break;

		switch (c) {
		case 0: /* getopt_long() flag */
			break;

		case 'd':
			dev_name = optarg;
			break;

		case 'h':
			usage(stdout, argc, argv);
			exit(EXIT_SUCCESS);

		case 'm':
			io = IO_METHOD_MMAP;
			break;

		case 'r':
			io = IO_METHOD_READ;
			break;

		case 'u':
			io = IO_METHOD_USERPTR;
			break;

		case 'o':
			out_buf++;
			break;

		case 'f':
			force_format++;
			break;

		case 'c':
			errno = 0;
			frame_count = strtol(optarg, NULL, 0);
			if (errno)
				errno_exit(optarg);
			break;

		case 'v':
			verbose = 1;
			break;

		default:
			usage(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}

////QT  QT  QT  Start ////////////////////////
    app = new QApplication(argc, argv);
    //ImageViewer imageViewer(argv[1]);
    imageViewer = new ImageViewer();
 //   imageViewer->show();

	//MyThread myThread;
    //QObject::connect(&myThread, SIGNAL(finished()), app, SLOT(quit()));
    //myThread.start();
//
//	ImageThread imageThread;
//    QObject::connect(&imageThread, SIGNAL(finished()), app, SLOT(quit()));
//    imageThread.start();

    return app->exec();
///QT     QT    QT  Stop /////////////////////////

/*
	open_device();
	init_device();
	start_capturing();
	mainloop();
   

	stop_capturing();
	uninit_device();
	close_device();
	fprintf(stderr, "Close Program\n");
//    return app.exec();
//	return 0;
*/
}
#endif
