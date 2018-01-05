#include <QApplication>
#ifdef __arm__
#include <QWSServer>
#endif
#include <QString>

#include "imageviewer.h"
#include "camera.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv, QApplication::GuiServer);

	if ( argc > 1 )
	{
		cam_width = QString(argv[1]).toInt();
		if ( cam_width <= 480 )
		{
			cam_width = 480;
			cam_height = 480;
		}
		else if ( cam_width <= 640 )
		{
			cam_width = 640;
			cam_height = 480;
		}
		else if ( cam_width <= 800 )
		{
			cam_width = 800;
			cam_height = 480;
		}
		else if ( cam_width <= 960 )
		{
			cam_width = 960;
			cam_height = 960;
		}
		else
		{
			cam_width = 1280;
			cam_height = 1024;
		}
	}
	qDebug("width %d, height %d", cam_width, cam_height);

	ImageViewer imageviewer;
	imageviewer.show();

	return a.exec();
}
