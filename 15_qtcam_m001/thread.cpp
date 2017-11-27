#include <thread.h>
#include <arm_neon.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MY_COUNT	4000
//#define QT_WIDTH	800
//#define QT_HEIGHT	480
QApplication 	*app;
QLabel 			*label;
ImageViewer 	*imageViewer;
MyThread 		*myThread;
//uchar           *pRGBData;
//unsigned int   *pRGBData;
unsigned char   *pRGBData;
extern struct buffer *buffers;


//bool nv21_to_rgb(unsigned char* rgb, unsigned char const* nv21, int width, int height);
bool nv21_to_rgb(unsigned char* rgb, unsigned char const* y, unsigned char const* uv, int width, int height) ;
inline int clip(int value, int min, int max)
{
    return(value > max ? max : value < min ? min : value);
}

//////////////////
template<typename trait>
bool decode_yuv_neon(unsigned char* out, unsigned char const* y, unsigned char const* uv, int width, int height, unsigned char fill_alpha=0xff)
{
    // pre-condition : width, height must be even
    if (0!=(width&1) || width<2 || 0!=(height&1) || height<2 || !out || !y || !uv)
        return false;

    // in & out pointers
    unsigned char* dst = out;

    // constants
    int const stride = width*trait::bytes_per_pixel;
    int const itHeight = height>>1;
    int const itWidth = width>>3;

    uint8x8_t const Yshift = vdup_n_u8(16);
    //int16x8_t const half = vdupq_n_u16(128);
    //int32x4_t const rounding = vdupq_n_s32(128);
    int32x4_t const tR = vdupq_n_s32(0);
    int32x4_t const tG = vdupq_n_s32(0);
    int32x4_t const tB = vdupq_n_s32(0);


    int32x4x2_t const R = vzipq_s32(tR, tR); // [tR0, tR0, tR1, tR1] [ tR2, tR2, tR3, tR3]
    int32x4x2_t const G = vzipq_s32(tG, tG); // [tG0, tG0, tG1, tG1] [ tG2, tG2, tG3, tG3]
    int32x4x2_t const B = vzipq_s32(tB, tB); // [tB0, tB0, tB1, tB1] [ tB2, tB2, tB3, tB3]

    // tmp variable
    uint16x8_t t;

// pixel block to temporary store 8 pixels
    typename trait::PixelBlock pblock = trait::init_pixelblock(fill_alpha);

    for (int j=0; j<itHeight; ++j, y+=width, dst+=stride) {
        for (int i=0; i<itWidth; ++i, y+=8, uv+=8, dst+=(8*trait::bytes_per_pixel)) {
            // Y00 = 298*(y-16) , Y01 = 298*(y-16)
			t = vmovl_u8(vqsub_u8(vld1_u8(y), Yshift));
            int32x4_t const Y00 = vmulq_n_u32(vmovl_u16(vget_low_u16(t)), 298);
            int32x4_t const Y01 = vmulq_n_u32(vmovl_u16(vget_high_u16(t)), 298);
            // Y10 = 298*(y-16) , Y11 = 298*(y-16)
            t = vmovl_u8(vqsub_u8(vld1_u8(y+width), Yshift));
            int32x4_t const Y10 = vmulq_n_u32(vmovl_u16(vget_low_u16(t)), 298);
            int32x4_t const Y11 = vmulq_n_u32(vmovl_u16(vget_high_u16(t)), 298);

            // trait::loadvu pack 4 sets of uv into a uint8x8_t, layout : { v0,u0, v1,u1, v2,u2, v3,u3 }
            //t = vsubq_s16((int16x8_t)vmovl_u8(trait::loadvu(uv)), half);

            // UV.val[0] : v0, v1, v2, v3
            // UV.val[1] : u0, u1, u2, u3
            //int16x4x2_t const UV = vuzp_s16(vget_low_s16(t), vget_high_s16(t));

            // tR : 128+409V
            // tG : 128-100U-208V 
            // tB : 128+516U 
            //int32x4_t const tR = vmlal_n_s16(rounding, UV.val[1], 409);
            //int32x4_t const tG = vmlal_n_s16(vmlal_n_s16(rounding, UV.val[1], -208), UV.val[0], -100);
            //int32x4_t const tB = vmlal_n_s16(rounding, UV.val[0], 516);
            //int32x4_t const tR = vmlal_n_s16(rounding, UV.val[1], 0);
            //int32x4_t const tG = vmlal_n_s16(vmlal_n_s16(rounding, UV.val[1], 0), UV.val[0], 0);
            //int32x4_t const tB = vmlal_n_s16(rounding, UV.val[0], 0);

            //int32x4x2_t const R = vzipq_s32(tR, tR); // [tR0, tR0, tR1, tR1] [ tR2, tR2, tR3, tR3]
            //int32x4x2_t const G = vzipq_s32(tG, tG); // [tG0, tG0, tG1, tG1] [ tG2, tG2, tG3, tG3]
            //int32x4x2_t const B = vzipq_s32(tB, tB); // [tB0, tB0, tB1, tB1] [ tB2, tB2, tB3, tB3]

            // upper 8 pixels
            trait::store_pixel_block(dst, pblock,
              vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(R.val[0], Y00)), vqmovun_s32(vaddq_s32(R.val[1], Y01))), 8),
              vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(G.val[0], Y00)), vqmovun_s32(vaddq_s32(G.val[1], Y01))), 8),
              vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(B.val[0], Y00)), vqmovun_s32(vaddq_s32(B.val[1], Y01))), 8));

            // lower 8 pixels
            trait::store_pixel_block(dst+stride, pblock,
              vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(R.val[0], Y10)), vqmovun_s32(vaddq_s32(R.val[1], Y11))), 8),
              vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(G.val[0], Y10)), vqmovun_s32(vaddq_s32(G.val[1], Y11))), 8),
              vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(B.val[0], Y10)), vqmovun_s32(vaddq_s32(B.val[1], Y11))), 8));
        }
    }
    return true;
}

//------------------------------------------------------------------------------
class NV21toRGB_neon {
public:
    enum { bytes_per_pixel = 3 };
    typedef uint8x8x3_t PixelBlock;
    static PixelBlock const init_pixelblock(unsigned char /*fill_alpha*/) {
        return uint8x8x3_t();
    }
    static uint8x8_t const loadvu(unsigned char const* uv) {
        return vld1_u8(uv);
    }
    static void store_pixel_block(unsigned char* dst, PixelBlock& pblock, uint8x8_t const& r, uint8x8_t const& g, uint8x8_t const& b) {
        pblock.val[0] = r;
        pblock.val[1] = g;
        pblock.val[2] = b;
        vst3_u8(dst, pblock);
    }
};

/*
bool nv21_to_rgb(unsigned char* rgb, unsigned char const* nv21, int width, int height) {
    return decode_yuv_neon<NV21toRGB_neon>(rgb, nv21, nv21+(width*height), width, height);
}
*/
bool nv21_to_rgb(unsigned char* rgb, unsigned char const* y, unsigned char const* uv, int width, int height) {
    return decode_yuv_neon<NV21toRGB_neon>(rgb, y, uv, width, height);
}






//////////////////


//int count=0;
ImageViewer::ImageViewer()
{

    label = new QLabel("eschoi",0);
    //label->resize(640,480);
    label->resize(QT_WIDTH,QT_HEIGHT);
    label->setText("No Image  ");
   // label->show();

	QScrollArea *scrollArea = new QScrollArea;
	setWindowTitle(tr("Qt Cam"));
	resize(QT_WIDTH+5,QT_HEIGHT+5);
	scrollArea->setWidget(label);
	setCentralWidget(scrollArea);

}

void ImageViewer::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Q)
	{
		//myLabelText->setText("You Pressed Key Q");
		qDebug() << "You Press q key ";
 		stop_capturing();
    	uninit_device();
    	close_device();
 		free(pRGBData);
    	qApp->quit();

	}
	else if(event->key() == Qt::Key_A){
		qDebug() << "A key => Up Gain";
		upControl_gain();
	}
	else if(event->key() == Qt::Key_D){
		qDebug() << "D key => Down Gain ";
		downControl_gain();
	}
	else if(event->key() == Qt::Key_S){
		qDebug() << "S key => Up Exposure";
		upControl_exp();
	}
	else if(event->key() == Qt::Key_F){
		qDebug() << "F key => Down Exposure";
		downControl_exp();
	}
	else if(event->key() == Qt::Key_K){
		qDebug() << "Up key => Up Row Start";
		upControl_rowStart();
	}
	else if(event->key() == Qt::Key_J){
		qDebug() << "Down key => Down Row Start";
		downControl_rowStart();
	}
	else if(event->key() == Qt::Key_H){
		qDebug() << "Left key => Left Col Start";
		upControl_colStart();
	}
	else if(event->key() == Qt::Key_L){
		qDebug() << "Right key => Right Col Start";
		downControl_colStart();
	}
	else
	{
		qDebug() << event->nativeScanCode();
	}
}
void ImageViewer::updateGUI(int index)
{

  	//int x, y, j;
    //int y0, u, y1, v, r, g, b,y2,y3;
	int width = QT_WIDTH;
	int height = QT_HEIGHT;
	//int istride = QT_WIDTH;  
    //long position = 0;

	void *ptr= (void *)buffers[index].start;
	unsigned char *in= (unsigned char *)ptr;
	//unsigned char *inYY= (unsigned char *)ptr;
	//unsigned char *inUV= (unsigned char *)(ptr+width*height);
#if 1

if((index % 2) == 0)
{
    g_led_on();
///NEON NEON NEON NEON NEON NEON FORMAT NV21
    nv21_to_rgb(pRGBData,in, in+height*width, width,  height);

    label->setPixmap(QPixmap::fromImage(QImage((unsigned char *)pRGBData,width,height,QImage::Format_RGB888)));

    g_led_off();
}


#else
if((index % 4) == 1)
{
	g_led_on();
// 이미지의 폭을 넘어가면 다음 라인으로 내려가도록 설정 

    for(y = 0; y < height; y += 2) {
        //for(j = 0, x = 0; j < (int)vinfo.xres * 2; j += 4, x += 2) {
        //for(j = 0, x = 0; j < istride; j += 2, x += 1) {
        for(j = 0, x = 0; j < istride; j += 2, x += 1) {
            y0 = inYY[j+0];
            y1 = inYY[j+1];
            y2 = inYY[width+j+0];
            y3 = inYY[width+j+1];
            // YUYV 성분을 분리 
            //u = inUV[j + 0] - 128;
            //v = inUV[j + 1] - 128;

			// YUV를 RGB로 전환 
           	//r = y0; //clip((298 * y0 + 409 * v + 128) >> 8, 0, 255);
           	//g = y0; //clip((298 * y0 - 100 * u - 208 * v + 128) >> 8, 0, 255);
           	//b = y0; //clip((298 * y0 + 516 * u + 128) >> 8, 0, 255);
           	//pRGBData[position]= (r << 16) + (g << 8) +b;
           	pRGBData[position]= (y0 << 16) + (y0 << 8) +y0;

			// YUV를 RGB로 전환 
           	//r = y2; //clip((298 * y2 + 409 * v + 128) >> 8, 0, 255);
           	//g = y2; //clip((298 * y2 - 100 * u - 208 * v + 128) >> 8, 0, 255);
           	//b = y2; //clip((298 * y2 + 516 * u + 128) >> 8, 0, 255);
           	//pRGBData[width+position]= (r << 16) + (g << 8) +b;
           	pRGBData[width+position]= (y2 << 16) + (y2 << 8) +y2;
           	position++;

			// YUV를 RGB로 전환 
           	//r = y1; //clip((298 * y1 + 409 * v + 128) >> 8, 0, 255);
           	//g = y1; //clip((298 * y1 - 100 * u - 208 * v + 128) >> 8, 0, 255);
           	//b = y1; //clip((298 * y1 + 516 * u + 128) >> 8, 0, 255);
           	//pRGBData[position]= (r << 16) + (g << 8) +b;
           	pRGBData[position]= (y1 << 16) + (y1 << 8) +y1;

			// YUV를 RGB로 전환 
           	//r = y3; //clip((298 * y3 + 409 * v + 128) >> 8, 0, 255);
           	//g = y3; //clip((298 * y3 - 100 * u - 208 * v + 128) >> 8, 0, 255);
           	//b = y3; //clip((298 * y3 + 516 * u + 128) >> 8, 0, 255);
           	//pRGBData[width+position]= (r << 16) + (g << 8) +b;
           	pRGBData[width+position]= (y3 << 16) + (y3 << 8) +y3;
           	position++;

        };
        inYY += (istride*2);
        //inUV += (istride*1);
        position += istride;
    };

}

// qDebug() << "updateGUI Sl(t : " << QString::number(value);
/*
	q_index= QString::number(index);
	p[0] = QString::number(in[0],16);
	p[1] = QString::number(in[1],16);
	p[2] = QString::number(in[2],16);
	p[3] = QString::number(in[3],16);
    qDebug() << "I=" << q_index << p[0] << p[1] << p[2] << p[3];
*/
//    label->setText("Buffer index= "+QString::number(index));
//    label->setText("Buffer index= "+QString::number(camBuffer.start));
if((index % 4) == 1) {
	label->setPixmap(QPixmap::fromImage(QImage((unsigned char *)pRGBData,width,height,QImage::Format_RGB32)));
g_led_off();
//	label->setPixmap(QPixmap::fromImage(QImage((unsigned int *)pRGBData,width,height,QImage::Format_RGB888)));
}
#endif
}

void MyThread::run()
{
	//int i,x,y;
	int count=MY_COUNT;
	int index;
	//int position=0;
	int width=QT_WIDTH;
	int height=QT_HEIGHT;
	qDebug() << __func__ << "in MyThread" << currentThread();

    pRGBData = (uchar *)malloc(width * height * 3 * sizeof(uchar));
//    pRGBData = (unsigned int *)malloc(width * height * sizeof(unsigned int));
    
	open_device();
    init_device();
    start_capturing();
 	while(count-- > 0)
	{
    	index = mainloop();
		emit updateThread(index);
    }
	stop_capturing();
    uninit_device();
    close_device();
	
 	free(pRGBData);

	qApp->quit();

}
///////////////////////////////////
int main(int argc, char **argv)
 {
	 app = new QApplication(argc, argv);
    imageViewer = new ImageViewer();
    imageViewer->show();
     qDebug() << __func__ << "in a main thread" << QThread::currentThread();

     
	myThread = new MyThread;
    QObject::connect(myThread, SIGNAL(updateThread(int)), imageViewer, SLOT(updateGUI(int)));
    myThread->start();

     return app->exec();
 }
