#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDatabase>

#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>
#include <QPieSlice>
#include <QLineSeries>

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
	void resizeEvent(QResizeEvent * event);

private slots:
	void on_loginButton_clicked();

private:
	Ui::MainWindow *ui;

	QSqlDatabase db;


	QChartView *chartViewBar;
	QChartView *chartViewPie;
	QChartView *chartViewLine;
};

#endif // MAINWINDOW_H
