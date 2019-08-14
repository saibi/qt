#ifndef TESTGUI_H
#define TESTGUI_H

#include "mainwindow.h"
#include <QTest>

class TestGui : public QObject
{
	Q_OBJECT
public:
	TestGui(QObject * parent = 0);

private slots:
	void controlButtonState();

private:
	MainWindow mMainWindow;
};

#endif // TESTGUI_H
