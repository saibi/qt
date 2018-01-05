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

	void handleClose();

	void handleCtrl(const QString & title, int id, int min = -4, int max = 4);

protected slots:
	void updateGUI(int count);

	void slot_clicked(const QPoint & pos);

private slots:
	void on_pushButton_exit_clicked();

	void on_pushButton_vflip_clicked();

	void on_pushButton_gain_clicked();

	void on_pushButton_exposure_clicked();

private:
	Ui::ImageViewer *ui;

	unsigned char   *pRGBData;

	MyThread myThread;

	bool m_stopFlag;

	int m_ctrlIdx;
	QList <int> m_ids;
};

#endif // IMAGEVIEWER_H
