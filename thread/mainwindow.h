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
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	virtual void timerEvent(QTimerEvent *);

private slots:
	void slot_workerProgress(QString name, unsigned int count);
	void slot_workerFinished();

	void on_pushButton_startWorker_clicked();

private:
	Ui::MainWindow *ui;

	QThread *m_thread;
	Worker *m_worker;

};

#endif // MAINWINDOW_H
