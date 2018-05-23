#ifndef A20DISP_H
#define A20DISP_H

#include <QObject>
#include <QImage>

#ifdef __linux__
#include <linux/fb.h>
#endif

#ifdef __arm_A20__
#include "sunxi_disp_ioctl.h"
#endif

class A20Disp : public QObject
{
Q_OBJECT

private:
	explicit A20Disp(QObject * parent = 0);
	~A20Disp();

	Q_DISABLE_COPY(A20Disp)

public:

	static A20Disp & instance()
	{
		static A20Disp disp;
		return disp;
	}


	// Layer
	bool init();
	void quit();

	enum LayerIndex
	{
		LAYER_CAM_BUF = 0,
		LAYER_FB_BACK_BUF,

		MAX_LAYER_IDX,
	};

	bool addCamBufLayer(int camWidth = 480, int camHeight = 480, int x = 0, int y = 0, int w = 480, int h = 480, bool scaling = false, int idx = LAYER_CAM_BUF);
	bool addFbBackBufLayer(int camWidth = 480, int camHeight = 480, int x = 0, int y = 0, int w = 480, int h = 480, bool forceScaler = false, int idx = LAYER_FB_BACK_BUF );

	bool showLayer(int idx, bool toBottom = true);
	bool hideLayer(int idx);
	bool removeLayer(int idx);
	void setCamBufAddr(unsigned int addr, int idx = LAYER_CAM_BUF);
	bool moveLayer(int idx, int x, int y, int width, int height);
	bool enableColorKey(int idx, unsigned int color = 0xff00);
	bool disableColorKey(int idx);
	bool isLayerAdded(int idx);
	bool toBottom(int idx);

	bool setLayerWindowEx(int idx, int camWidth, int camHeight, int srcX, int srcY, int srcW, int srcH, int dispX, int dispY, int dispW, int dispH, bool checkFd = true);
	bool setLayerWindow(int idx, int camWidth, int camHeight, int x, int y, int w, int h, bool scaling = false, bool checkFd = true);

	bool isLayerVisible(int idx) { return (m_layer[idx].flag & LAYER_FLAG_SHOW) ? true : false; }
	bool layerVideoStartStop(bool flag, int idx = LAYER_CAM_BUF);

	// frame buffer
	enum FrameBufferAreaIndex
	{
		FB_CUR = -1,
		FB_MAIN = 0,
		FB_BACK
	};

#ifdef A20_FB_TEST
	void drawImg(int x, int y, const QImage & img, enum FrameBufferAreaIndex fbIdx = FB_CUR);
	void fillBox(int x = 0, int y = 0, int w = 100, int h = 100, unsigned int c = 0, enum FrameBufferAreaIndex fbIdx = FB_CUR);
	bool toggleFbArea(enum FrameBufferAreaIndex fbIdx);
	void copyFb(enum FrameBufferAreaIndex srcIdx);
#endif
	void drawCam(int x, int y, unsigned char * camData, int width, int height, enum FrameBufferAreaIndex fbIdx = FB_CUR);
	QImage saveFB(enum FrameBufferAreaIndex srcIdx);

#ifdef A20_G2D_TEST
	// g2d
	bool g2d_fill(int x, int y, int w, int h, unsigned int color);
	bool g2d_blit(int dest_x, int dest_y, int src_x, int src_y, int w, int h);
#endif

private:

	struct FileDescriptor
	{
		int disp;
		int fb;
#ifdef A20_G2D_TEST
		int g2d;
#endif
	} m_fd;

	enum ScreenIndex
	{
		SCREEN_0 = 0, // /dev/fb0
	};

	struct ScreenInfo
	{
		int layerId;

		char * fb_addr; // mmapped address
#ifdef __linux__
		struct fb_var_screeninfo fb_var;
		struct fb_fix_screeninfo fb_fix;
#endif
		int fbDepthBytes;

	} m_screen;

	struct LayerInfo {
		int id;
		int flag;
#ifdef __arm_A20__
		__disp_layer_info_t param;
#endif
	} m_layer[MAX_LAYER_IDX];
	enum LayerFlagBit
	{
		LAYER_FLAG_SHOW = 0x01,
	};

	bool m_scalerUsed;

protected:
	void clearVars();
	bool checkDispVer();

#ifdef DBG_LAYER_PARAM
	void pr_layer_param(__disp_layer_info_t * p);
	void dumpLayerParam(int idx);
#endif
};

#endif // A20DISP_H
