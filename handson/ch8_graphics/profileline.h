#ifndef PROFILELINE_H
#define PROFILELINE_H

#include <QGraphicsLineItem>
#include <QPen>

class ProfileLine : public QGraphicsLineItem
{
public:
	ProfileLine(QGraphicsItem * parent = nullptr);

	void initLine(QGraphicsItem * start, QGraphicsItem * end);
	void updateLine();

	QGraphicsItem * startBox;
	QGraphicsItem * endBox;
};

#endif // PROFILELINE_H
