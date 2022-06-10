#include "receiver.h"
#include <QDebug>
#include <QKeyEvent>

Receiver::Receiver(QWidget *parent) : QWidget(parent)
{
	qDebug() << "DBG power button press/release test";
}

void Receiver::keyPressEvent(QKeyEvent *event)
{
	if ( event->key() == Qt::Key_F1 )
		qDebug() << "DBG keyPressEvent" << event->key();

	QWidget::keyPressEvent(event);
}


void Receiver::keyReleaseEvent(QKeyEvent *event)
{
	if ( event->key() == Qt::Key_F1)
		qDebug() << "DBG keyReleaseEvent" << event->key();

	QWidget::keyReleaseEvent(event);
}
