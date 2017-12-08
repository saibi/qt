#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

#include "camera.h"
#include "mythread.h"

#define QT_WIDTH    FORCED_WIDTH
#define QT_HEIGHT   FORCED_HEIGHT

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer();
	~ImageViewer();

public slots:
	void updateGUI(int count);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QLabel *label;
	QScrollArea *scrollArea;

	unsigned char   *pRGBData;

	MyThread myThread;

	bool m_stopFlag;
};
#endif 


