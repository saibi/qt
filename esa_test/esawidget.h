#ifndef ESAWIDGET_H
#define ESAWIDGET_H

#include <QWidget>

namespace Ui {
class ESAWidget;
}

class ESAWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ESAWidget(QWidget *parent = 0);
	~ESAWidget();

private:
	Ui::ESAWidget *ui;
};

#endif // ESAWIDGET_H
