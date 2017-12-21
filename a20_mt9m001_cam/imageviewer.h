#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include "camera.h"
#include "mythread.h"

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	explicit ImageViewer(QWidget *parent = 0);
	~ImageViewer();

public slots:
	void updateGUI(int count);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	Ui::ImageViewer *ui;

	unsigned char   *pRGBData;

	MyThread myThread;

	bool m_stopFlag;
};

#endif // IMAGEVIEWER_H
