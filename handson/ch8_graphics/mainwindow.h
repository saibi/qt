#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>

#include <QDebug>

#include <QFont>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void selectionChanged();

private slots:
	void on_pushButton_add_clicked();

private:
	Ui::MainWindow *ui;

	QGraphicsScene * scene;
};

#endif // MAINWINDOW_H
