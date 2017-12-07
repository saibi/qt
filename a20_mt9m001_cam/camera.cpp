/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 *
 *	This program were got from V4L2 API, Draft 0.20
 *		available at: http://v4l2spec.bytesex.org/
 */
#include "camera.h"

#include <assert.h>
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

static int verbose = 1;

#define PR_DEBUG(fmt, arg...) \
		if (verbose) fprintf(stderr, fmt, ##arg)


struct buffer          *buffers;
static char            *dev_name;
static enum io_method   cam_io_method = IO_METHOD_MMAP;
static int              fd = -1;
static int              force_format=1;

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
    struct v4l2_control control_s;
    int err;

    control_s.id = control;
    if ((err = ioctl(fd, VIDIOC_G_CTRL, &control_s)) < 0) {
		PR_DEBUG("ioctl get control error\n");
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
		  PR_DEBUG("ioctl set control error\n");
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
		PR_DEBUG("ioctl query cotrol error\n");
		return -1;
	}
    max  = queryctrl.maximum;
    step = queryctrl.step;
    getControl(control , &current);
    current += step;
    if (current <= max) {
        control_s.id = control;
        control_s.value = current;
        if ((err = ioctl(fd, VIDIOC_S_CTRL, &control_s)) < 0) {
			PR_DEBUG("ioctl set control error\n");
            return -1;
        }
		PR_DEBUG("Control name:%s set to value:%d\n", queryctrl.name, control_s.value);
    }
    else {
		PR_DEBUG("Control name:%s already has max value:%d \n", queryctrl.name, max);
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
		PR_DEBUG("ioctl query cotrol error\n");
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
			  PR_DEBUG("ioctl set control error\n");
              return -1;
		}
		PR_DEBUG("Control name:%s set to value:%d\n", queryctrl.name, control_s.value);
	}
    else
		PR_DEBUG("Control name:%s already has min value:%d \n", queryctrl.name, min);
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

static void process_image(const void *p, int size,int index_buf)
{
	unsigned char *in = (unsigned char *)p;

	fprintf(stderr,"\nIndex=%d size %d| ",index_buf, size);
	fprintf(stderr, "%02x %02x %02x %02x ",in[0],in[1],in[2],in[3]);
	fprintf(stderr, "%02x %02x %02x %02x ",in[4],in[5],in[6],in[7]);
}

static int read_frame(void)
{
	struct v4l2_buffer buff;
	unsigned int i;

	switch (cam_io_method) {
	case IO_METHOD_READ:
		if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
			switch (errno) {
			case EAGAIN:
				fprintf(stderr, "EAGAIN ");
				return -1;

			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */
			default:
				errno_exit("read");
			}
		}

		process_image(buffers[0].start, buffers[0].length,0);
		return 0;
		break;

	case IO_METHOD_MMAP:

		CLEAR(buff);

		buff.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buff.memory = V4L2_MEMORY_MMAP;

		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buff)) {
			switch (errno) {
			case EAGAIN:
				fprintf(stderr, "EAGAIN ");
				return -1;

			case EIO:
				printf("EIO ");
				/* Could ignore EIO, see spec. */
				/* fall through */

			default:
				errno_exit("VIDIOC_DQBUF_RF");
			}
		}

		if (-1 == xioctl(fd, VIDIOC_QBUF, &buff))
			errno_exit("VIDIOC_QBUF_RF");

		return buff.index;
		break;

	case IO_METHOD_USERPTR:
		CLEAR(buff);

		buff.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buff.memory = V4L2_MEMORY_USERPTR;

		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buff)) {
			switch (errno) {
			case EAGAIN:
				fprintf(stderr, "EAGAIN ");
				return -1;

			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */

			default:
				errno_exit("VIDIOC_DQBUF");
			}
		}

		for (i = 0; i < MAX_CAM_BUFFER; ++i)
			if (buff.m.userptr == (unsigned long)buffers[i].start
			    && buff.length == buffers[i].length)
				break;

		assert(i < MAX_CAM_BUFFER);

		process_image((void *)buff.m.userptr, buff.bytesused, i);

		if (-1 == xioctl(fd, VIDIOC_QBUF, &buff))
			errno_exit("VIDIOC_QBUF");

		return i;
		break;
	}
	return -1;
}

int mainloop(void)
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

	if ( r < 0 )
	{
		fprintf(stderr, "select = %d\n", errno);
		return -1;
	}
	else if ( 0 == r )
	{
		fprintf(stderr, "select timeout\n");
		return -1;
	}

	return read_frame();
}

void stop_capturing(void)
{
	enum v4l2_buf_type type;

	PR_DEBUG("%s: called!\n", __func__);
	switch (cam_io_method) {
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
	enum v4l2_buf_type type;

	switch (cam_io_method) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;

	case IO_METHOD_MMAP:
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
			errno_exit("VIDIOC_STREAMON");
		break;

	case IO_METHOD_USERPTR:

		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
			errno_exit("VIDIOC_STREAMON");
		break;
	}
}

void uninit_device(void)
{
	unsigned int i;

	PR_DEBUG("%s: called!\n", __func__);

	switch (cam_io_method) {
	case IO_METHOD_READ:
		free(buffers[0].start);
		break;

	case IO_METHOD_MMAP:
		for (i = 0; i < MAX_CAM_BUFFER; ++i)
			if (-1 == munmap(buffers[i].start, buffers[i].length))
				errno_exit("munmap");
		break;

	case IO_METHOD_USERPTR:
		for (i = 0; i < MAX_CAM_BUFFER; ++i)
			free(buffers[i].start);
		break;
	}

	free(buffers);
}

static void init_read(unsigned int buffer_size)
{
	PR_DEBUG("%s: called!\n", __func__);

	buffers = (buffer *)calloc(1, sizeof(struct buffer));

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
	struct v4l2_buffer v4l_buf[MAX_CAM_BUFFER];
	struct v4l2_requestbuffers req;
	int i_buffers=0;

	/////////Init Frame Buffer/////
	/* 프레임버퍼를 오픈한다. */
	PR_DEBUG("Init_mmap\r\n");
	CLEAR(req);

	req.count = MAX_CAM_BUFFER ;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	PR_DEBUG("Init_mmap req.count=%d \r\n",req.count);
	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
				 "memory mapping\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}

	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n",
			 dev_name);
		exit(EXIT_FAILURE);
	}

	buffers = (buffer *)calloc(req.count, sizeof(struct buffer));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (i_buffers = 0; i_buffers < (int)req.count;i_buffers++ ) 
	{

		req.count = MAX_CAM_BUFFER ;

		CLEAR(v4l_buf[i_buffers]);

		v4l_buf[i_buffers].type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l_buf[i_buffers].memory      = V4L2_MEMORY_MMAP;
		v4l_buf[i_buffers].index       = i_buffers;
		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &v4l_buf[i_buffers]))
			errno_exit("VIDIOC_QBUF");

		PR_DEBUG("\tbuf.m.offset: %d\n", v4l_buf[i_buffers].m.offset);
		PR_DEBUG("\tbuf.length: %d\n", v4l_buf[i_buffers].length);
		PR_DEBUG("\ti_buffers =%d req.count=%d \n", i_buffers, req.count);

		buffers[i_buffers].length = v4l_buf[i_buffers].length;
		buffers[i_buffers].start =
			mmap(NULL /* start anywhere */,
				  v4l_buf[i_buffers].length,
			      PROT_READ | PROT_WRITE /* required */,
			      MAP_SHARED /* recommended */,
				  fd, v4l_buf[i_buffers].m.offset);

		PR_DEBUG("buf Index = %d start=%p\n", i_buffers, buffers[i_buffers].start);
		if (MAP_FAILED == buffers[i_buffers].start)
			errno_exit("mmap");
		if (-1 == xioctl(fd, VIDIOC_QBUF, &v4l_buf[i_buffers]))
			errno_exit("VIDIOC_QBUF");
	}
		PR_DEBUG("END Init MMap\n");

}

static void init_userp(unsigned int buffer_size)
{
	struct v4l2_requestbuffers req;
	int i;

	PR_DEBUG("init_userp\n");

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

	buffers = (buffer *)calloc(MAX_CAM_BUFFER, sizeof(struct buffer));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < MAX_CAM_BUFFER; ++i) {
		buffers[i].length = buffer_size;
		buffers[i].start = malloc(buffer_size);

		if (!buffers[i].start) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}
	}
}

void init_device(void)
{
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	unsigned int min;
	int i;

	PR_DEBUG("%s: called!\n", __func__);

	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n",
				 dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_QUERYCAP");
		}
	}

	PR_DEBUG("\tdriver: %s\n"
		 "\tcard: %s \n"
		 "\tbus_info: %s\n",
			cap.driver, cap.card, cap.bus_info);
	PR_DEBUG("\tversion: %u.%u.%u\n",
			(cap.version >> 16) & 0xFF,
			(cap.version >> 8) & 0xFF,
			cap.version & 0xFF);
	PR_DEBUG("\tcapabilities: 0x%08x\n", cap.capabilities);

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is no video capture device\n",
			 dev_name);
		exit(EXIT_FAILURE);
	}

	switch (cam_io_method) {
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
		PR_DEBUG("\tcropcap.type: %d\n", cropcap.type);
		PR_DEBUG("\tcropcap.bounds.left: %d\n", cropcap.bounds.left);
		PR_DEBUG("\tcropcap.bounds.top: %d\n", cropcap.bounds.top);
		PR_DEBUG("\tcropcap.bounds.width: %d\n", cropcap.bounds.width);
		PR_DEBUG("\tcropcap.bounds.height: %d\n", cropcap.bounds.height);

		PR_DEBUG("\tcropcap.pixelaspect.numerator: %d\n", cropcap.pixelaspect.numerator);
		PR_DEBUG("\tcropcap.pixelaspect.denominator: %d\n", cropcap.pixelaspect.denominator);
		PR_DEBUG("\n");
		
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
				PR_DEBUG("\tcropping not supported\n");
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
		PR_DEBUG("\tSUPPORT FORMAT: %c,%c,%c,%c\n",
				fmtdesc.pixelformat & 0xFF,
				(fmtdesc.pixelformat >> 8) & 0xFF,
				(fmtdesc.pixelformat >> 16) & 0xFF,
				(fmtdesc.pixelformat >> 24) & 0xFF
				);
	}		

	CLEAR(fmt);
	
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (force_format) {
		fmt.fmt.pix.width       = FORCED_WIDTH;
		fmt.fmt.pix.height      = FORCED_HEIGHT;
		fmt.fmt.pix.pixelformat = FORCED_FORMAT;
		fmt.fmt.pix.field       = FORCED_FIELD;

		PR_DEBUG("\tfmt.fmt.pix.pixelformat: %c,%c,%c,%c\n",
				fmt.fmt.pix.pixelformat & 0xFF,
				(fmt.fmt.pix.pixelformat >> 8) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 16) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 24) & 0xFF
				);
		PR_DEBUG("\n");

		PR_DEBUG("\tVIDOC_S_FMT Start\n");
		if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
			errno_exit("VIDIOC_S_FMT");

		/* Note VIDIOC_S_FMT may change width and height. */
	} else {
		/* Preserve original settings as set by v4l2-ctl for example */
		if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
			errno_exit("VIDIOC_G_FMT");

		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		PR_DEBUG("\tfmt.fmt.pix.pixelformat: %c,%c,%c,%c\n",
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

	switch (cam_io_method) {
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
	PR_DEBUG("%s: called!\n", __func__);

	if (-1 == close(fd))
		errno_exit("close");
	fd = -1;
}

void open_device(void)
{
	struct stat st;
	
	dev_name = (char *) VIDEODEV;

	PR_DEBUG("%s: called!\n", __func__);

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
}
