#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT
	
public:
	explicit Widget(QWidget *parent = 0);
	~Widget();
	
protected:
	void showEvent(QShowEvent *);
	void timerEvent(QTimerEvent *);

private slots:
	void on_pushButton_clicked();

	void on_pushButton_left_clicked();

	void on_pushButton_right_clicked();

	void on_pushButton_down_clicked();

	void on_pushButton_up_clicked();

	void on_pushButton_left_bg_clicked();

	void on_pushButton_right_bg_clicked();

	void on_pushButton_down_bg_clicked();

	void on_pushButton_up_bg_clicked();

	void on_pushButton_merge_clicked();

private:
	Ui::Widget *ui;
};

#endif // WIDGET_H
