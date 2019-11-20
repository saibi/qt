#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebEngineView>
#include <QWebChannel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void paintEvent(QPaintEvent *event);

	void loadPage();


	Q_INVOKABLE void doSomething();

private slots:
	void on_pushButton_back_clicked();

	void on_pushButton_forward_clicked();

	void on_pushButton_refresh_clicked();

	void on_pushButton_go_clicked();

	void on_lineEdit_returnPressed();

	void webviewLoading(int progress);
	void webviewLoaded();

	void on_pushButton_pdf_clicked();

	void on_pushButton_hello_clicked();

private:
	Ui::MainWindow *ui;

	QWebEngineView *webview;
};

#endif // MAINWINDOW_H
