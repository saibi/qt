#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include <QDialog>

namespace Ui {
class ButtonWidget;
}

class ButtonWidget : public QDialog
{
	Q_OBJECT

public:
	explicit ButtonWidget(QWidget *parent = 0);
	~ButtonWidget();

private slots:
	void on_pushButton_clicked();

private:
	Ui::ButtonWidget *ui;
};

#endif // BUTTONWIDGET_H
