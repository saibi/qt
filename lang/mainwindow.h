#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
	void on_actionEnglish_triggered();

	void on_actionKorean_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
