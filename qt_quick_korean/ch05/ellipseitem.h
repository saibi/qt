#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H

#include <QQuickPaintedItem>

class EllipseItem : public QQuickPaintedItem
{
	Q_OBJECT

public:
	EllipseItem(QQuickItem *parent = nullptr);

	void paint(QPainter *painter);
};

#endif // ELLIPSEITEM_H
