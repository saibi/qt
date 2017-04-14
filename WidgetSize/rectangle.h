#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QWidget>

class Rectangle : public QWidget
{
	Q_OBJECT
public:
	explicit Rectangle(QWidget *parent = 0);

protected:
	void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // RECTANGLE_H
