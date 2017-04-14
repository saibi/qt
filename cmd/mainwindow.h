#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dlg_progress.h"
#include "cmdthread.h"
#include "progressthread.h"

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
	void on_pushButton_clicked();
	void cmdFinished();

private:
	Ui::MainWindow *ui;
	Dlg_Progress m_dlgProgress;
	CmdThread m_thread;
	ProgressThread m_pbar;
};

#endif // MAINWINDOW_H
