#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>
#include <QDebug>

class MyClass : public QObject
{
	Q_OBJECT

public:

public slots:
	void cppSlot(const QString & msg) {
		qDebug() << "Called the c++ slot with message: " << msg;
	}
};


#endif // MYCLASS_H
