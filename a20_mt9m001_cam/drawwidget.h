#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include <QPaintEvent>

class DrawWidget : public QWidget
{
	Q_OBJECT

public:
	explicit DrawWidget(QWidget *parent = 0);

protected:
	virtual void paintEvent(QPaintEvent *e);

};

#endif // DRAWWIDGET_H
