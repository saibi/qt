#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QMoveEvent>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool eventFilter(QObject *obj, QEvent *event)
	{
		if ( event->type() == QEvent::Move )
		{
			QMoveEvent * moveEvent = static_cast<QMoveEvent*>(event);
			qDebug() << "Move event:" << moveEvent->pos();
		}
		else
		{
			qDebug() << "Event type:" << event->type();
		}
		return QMainWindow::eventFilter(obj, event);
	}


private slots:
	void on_pushButton_clicked();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
