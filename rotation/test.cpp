#include "test.h"
#include <QPainter>

Test::Test(QWidget *parent) :
	QWidget(parent)
{
}

void Test::paintEvent(QPaintEvent *)
{
	QPainter painter(this);


	painter.setPen( QPen(Qt::black, 1.0));
	painter.setBrush(QBrush(Qt::transparent, Qt::SolidPattern));
	painter.drawRect(0, 0, this->width() - 1, this->height() -1 );

	painter.setPen( QPen(Qt::blue, 1.0));
	painter.drawLine(QPoint(20,50), QPoint(70, 50));

	painter.rotate(45.0);
	painter.setPen( QPen(Qt::red, 1.0));
	painter.drawLine(QPoint(20,50), QPoint(70, 50));


	painter.setPen( QPen(Qt::green, 1.0));
	painter.drawLine(QPoint(20,10), QPoint(70, 10));

}
