#include "drawwidget.h"
#include <QPainter>

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{

}

void DrawWidget::paintEvent(QPaintEvent *e)
{
	QWidget::paintEvent(e);


	QPainter p(this);
	QPen pen;
	pen.setColor(Qt::red);
	pen.setStyle(Qt::DashLine);
	p.setPen(pen);

	QPoint center = this->rect().center();
	QRect rect(center.x() - 100, center.y() - 100, 200, 200);
	p.drawRect(rect);

}
