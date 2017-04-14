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
	void paintEvent(QPaintEvent *);

private slots:
	void on_pushButton_inc_clicked();

	void on_pushButton_dec_clicked();

	void on_pushButton_left_clicked();

	void on_pushButton_right_clicked();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
