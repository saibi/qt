#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "task.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void updateStatus();

public slots:
	void slotAddTask();
	void slotRemoveTask(Task * task);
	void slotTaskStatusChanged(Task *task);

private:
	Ui::MainWindow *ui;

	QVector<Task*> mTasks;
};

#endif // MAINWINDOW_H
