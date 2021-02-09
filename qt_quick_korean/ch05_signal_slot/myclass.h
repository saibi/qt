#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>
#include <QDebug>
#include <QQuickItem>

class MyClass : public QObject
{
	Q_OBJECT

public:

public slots:
	void cppSlot(const QVariant &v) {
		QQuickItem *item = qobject_cast<QQuickItem*>(v.value<QObject*>());
		qDebug() << "Item size: " << item->width() << item->height();
	}
};


#endif // MYCLASS_H
