#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void positionUpdated(const QGeoPositionInfo &info);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
