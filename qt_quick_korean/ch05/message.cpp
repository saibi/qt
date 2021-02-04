#include "message.h"
#include <QDebug>

Message::Message(QObject *parent) : QObject(parent)
{
	qDebug() << "Message() Construction";
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
