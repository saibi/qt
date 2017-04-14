#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dialog.h>
#include <dialog2.h>

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
	void on_pushButton_run_clicked();

	void on_pushButton_run2_clicked();

private:
	Ui::MainWindow *ui;


	Dialog *m_dlg;
	Dialog *m_dlg2;
};

#endif // MAINWINDOW_H
