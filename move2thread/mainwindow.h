#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QThread>
#include "worker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void handleStop();
	void handleStart();

private:
	Ui::MainWindow *ui;

	Worker mWorker;
	QThread mWorkerThread;
};

#endif // MAINWINDOW_H
