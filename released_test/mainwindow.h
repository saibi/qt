#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
	void showEvent(QShowEvent *);

private slots:
	void on_pushButton_pressed();

	void on_pushButton_released();

	void slot_hide();
private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
