#ifndef TEST_H
#define TEST_H

#include <QWidget>

class Test : public QWidget
{
	Q_OBJECT
public:
	explicit Test(QWidget *parent = 0);

protected:
	virtual void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // TEST_H
