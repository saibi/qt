#include "message.h"
#include <QDebug>
#include <QTimer>

Message::Message(QObject *parent) : QObject(parent)
{
	qDebug() << "Message() Construction";
	QTimer::singleShot(3000, this, SLOT(timerTimeout()));
}

bool Message::postMessage(const QString &msg)
{
	qDebug() << "[C++] call postMessage method : " << msg;
	return true;
}

void Message::refresh()
{
	qDebug() << "Called the c++ slot";
}

void Message::timerTimeout()
{
	emit newMessagePosted("I am a boy");
}
