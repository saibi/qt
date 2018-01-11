#ifndef _TEST_H_
#define _TEST_H_

#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include "ticker.h"
#include "thread.h"

class Test : public QDialog
{
	Q_OBJECT
public:
	Test(QWidget *parent = 0);

	QLabel *label;
	Ticker *ticker;

	bool eventFilter(QObject *target, QEvent *event);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void startOrStopThreadA();
	void startOrStopThreadB();

private:
	Thread threadA;
	Thread threadB;

	QPushButton *threadAButton;
	QPushButton *threadBButton;
	QPushButton *quitButton;

};
#endif

