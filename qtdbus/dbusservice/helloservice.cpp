#include "helloservice.h"

#include <QDebug>

QString HelloService::sayHello(const QString & name)
{
	qDebug().noquote() << name << "is here!";
	return QString("Hello %1!").arg(name);
}
