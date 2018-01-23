#ifndef A20DISP_H
#define A20DISP_H

#include <QObject>
#include <QImage>

#include <linux/fb.h>
#include "sunxi_disp_ioctl.h"

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


	bool init();
	void quit();

	bool addCamScalerLayer(int camWidth = 480, int camHeight = 480, int x = 0, int y = 0, int w = 480, int h = 480, int idx = LAYER_CAM_SCALER);
	bool addCamFbLayer(int x = 0, int y = 0, int w = 480, int h = 480, int idx = LAYER_CAM_FB );

	bool showLayer(int idx);
	bool hideLayer(int idx);

	bool removeLayer(int idx);

	void setCamScalerAddr(unsigned int addr, int idx = LAYER_CAM_SCALER);
	bool moveLayer(int idx, int x, int y, int width, int height);

	bool enableColorKey(int idx, unsigned int color);
	bool disableColorKey(int idx);

	enum LayerIndex
	{
		LAYER_CAM_SCALER = 0,
		LAYER_CAM_FB,

		MAX_LAYER_IDX,
	};


	// frame buffer
	enum FrameBufferAreaIndex
	{
		FB_CUR = -1,
		FB_MAIN = 0,
		FB_BACK
	};

	void drawImg(int x, int y, const QImage & img, enum FrameBufferAreaIndex fbIdx = FB_CUR);
	void fillBox(int x = 0, int y = 0, int w = 100, int h = 100, unsigned int c = 0, enum FrameBufferAreaIndex fbIdx = FB_CUR);
	bool toggleFbArea(enum FrameBufferAreaIndex fbIdx);
	void copyFb(enum FrameBufferAreaIndex srcIdx);

	void drawCam(int x, int y, unsigned char * camData, int width, int height, enum FrameBufferAreaIndex fbIdx = FB_CUR);

private:

	struct FileDescriptor
	{
		int disp;
		int fb;
	} m_fd;

	enum ScreenIndex
	{
		SCREEN_0 = 0, // /dev/fb0
	};

	struct ScreenInfo
	{
		int layerId;

		char * fb_addr; // mmapped address

		struct fb_var_screeninfo fb_var;
		struct fb_fix_screeninfo fb_fix;
		int fbDepthBytes;

	} m_screen;

	struct LayerInfo {
		int id;
		__disp_layer_info_t param;
	} m_layer[MAX_LAYER_IDX];

protected:
	void clearVars();
	bool checkDispVer();


	//bool addWorkLayer(int idx);


};

#endif // A20DISP_H
