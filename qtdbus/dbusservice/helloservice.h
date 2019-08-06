#ifndef HELLOSERVICE_H
#define HELLOSERVICE_H

#include <QObject>

class HelloService : public QObject
{
	Q_OBJECT

public slots:
	QString sayHello(const QString &name);

};

#endif // HELLOSERVICE_H
