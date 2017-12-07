#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <linux/videodev2.h>

struct buffer {
	void   *start;
	size_t  length;
};

extern struct buffer *buffers;

#define VIDEODEV "/dev/video0"
#define FORCED_FORMAT 	V4L2_PIX_FMT_GREY
#define FORCED_FIELD  	V4L2_FIELD_NONE
#define MAX_CAM_BUFFER	4

#define FORCED_WIDTH    640
#define FORCED_HEIGHT   480


#define CLEAR(x) memset(&(x), 0, sizeof(x))

enum io_method {
		IO_METHOD_READ,
		IO_METHOD_MMAP,
		IO_METHOD_USERPTR,
};

void open_device(void);
void init_device(void);
void start_capturing(void);
int  mainloop(void);
void stop_capturing(void);
void uninit_device(void);
void close_device(void);

int getControl(int control , int *value);
int setControl(int control, int value);
int upControl(int control);
int downControl(int control);

void upControl_gain(void);
void downControl_gain(void);
void upControl_exp(void);
void downControl_exp(void);
void upControl_rowStart(void);
void downControl_rowStart(void);
void upControl_colStart(void);
void downControl_colStart(void);



#endif // CAMERA_H

