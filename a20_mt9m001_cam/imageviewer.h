#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QPoint>

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


protected:
	void keyPressEvent(QKeyEvent *event);

protected slots:
	void updateGUI(int count);

	void slot_clicked(const QPoint & pos);

private:
	Ui::ImageViewer *ui;

	unsigned char   *pRGBData;

	MyThread myThread;

	bool m_stopFlag;
};

#endif // IMAGEVIEWER_H
