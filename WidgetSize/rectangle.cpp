#include "rectangle.h"

#include <QPainter>

Rectangle::Rectangle(QWidget *parent) :
	QWidget(parent)
{
}

void Rectangle::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));

	painter.drawLine(this->rect().topLeft(), this->rect().topRight());
	painter.drawLine(this->rect().bottomLeft(), this->rect().bottomRight());
	painter.drawLine(this->rect().topLeft(), this->rect().bottomLeft());
	painter.drawLine(this->rect().topRight(), this->rect().bottomRight());

	painter.drawLine(this->rect().topLeft(), this->rect().bottomRight());
	painter.drawLine(this->rect().bottomLeft(), this->rect().topRight());
}
