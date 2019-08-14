#include "testgui.h"

#include <QtTest/QtTest>
#include <QPushButton>

TestGui::TestGui(QObject *parent) :
	QObject(parent),
	mMainWindow()
{
	QTestEventLoop::instance().enterLoop(1);

}


void TestGui::controlButtonState()
{
	QPushButton * stopButton = mMainWindow.findChild<QPushButton*>("stopButton");
	QPushButton * playButton = mMainWindow.findChild<QPushButton*>("playButton");
	QPushButton * recordButton = mMainWindow.findChild<QPushButton*>("recordButton");

	QTest::mouseClick(recordButton, Qt::LeftButton);

	QCOMPARE(stopButton->isEnabled(), true);
	QCOMPARE(playButton->isEnabled(), false);
	QCOMPARE(recordButton->isEnabled(), false);
}
