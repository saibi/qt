#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

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

	void on_pushButton_exit_clicked();

	void on_pushButton_clear_clicked();

	void on_pushButton_buffer_clicked();

	void on_pushButton_copy_clicked();

private:
	Ui::MainWindow *ui;

	QPixmap m_aoa;
	QPixmap m_apink;
	QPixmap m_so;
};

#endif // MAINWINDOW_H
