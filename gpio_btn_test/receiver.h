#ifndef RECEIVER_H
#define RECEIVER_H

#include <QWidget>

class Receiver : public QWidget
{
	Q_OBJECT

public:
	Receiver(QWidget *parent = 0);

protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

};

#endif // RECEIVER_H
