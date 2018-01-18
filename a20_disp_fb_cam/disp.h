#ifndef DISP_H
#define DISP_H

#include <QObject>

#include <linux/fb.h>
#include "sunxi_disp_ioctl.h"

#define DISP_SCREEN_0 0

class Disp  : public QObject
{
Q_OBJECT

private:
	explicit Disp(QObject * parent = 0);
	~Disp();
	Q_DISABLE_COPY(Disp)

public:
	static Disp & instance()
	{
		static Disp disp;
		return disp;
	}


	bool init(int x = 0, int y = 0, int width = 480, int height = 480);

	void quit();

	void set_para(int width, int height);
	void start();
	void stop();
	int set_addr(int width, int height, unsigned int *addr);

	bool enableColorKey(int screenLayer, unsigned int color);
	bool disableColorKey();

protected:
	void clearMemberVars();
	int wrap_ioctl(int fd, unsigned long req, unsigned int *addr);

private:
	int m_fdDisp;

	int m_layer;
	int m_screenWidth;
	int m_screenHeight;

	__disp_layer_info_t m_layerParam;
};

#endif // DISP_H
