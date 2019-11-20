#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	virtual void paintEvent(QPaintEvent *event);

	void paintImage(QString fileName, int x, int y);

private slots:
	void on_pushButton_browse_clicked();

	void on_pushButton_save_clicked();

	void on_horizontalSlider_scale_valueChanged(int value);

	void on_horizontalSlider_h_valueChanged(int value);

	void on_horizontalSlider_v_valueChanged(int value);

private:
	Ui::MainWindow *ui;

	bool canDraw;
	QPixmap *pix;
	QSize imageSize;
	QSize drawSize;
	QPoint drawPos;
};

#endif // MAINWINDOW_H
