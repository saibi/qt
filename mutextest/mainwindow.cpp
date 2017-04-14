#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "notification.h"
#include "sdobserver.h"
#include "sdobserver2.h"
#include "sdobserver3.h"

#include <qmath.h>

#include "dataconverter.h"

#include "motorhandler.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	DataConverter::makeCirclePolygon(m_circleData, 25.0);

	connect(&SdObserver2::instance(), SIGNAL(signalSdMounted()), this, SLOT( slot_sdMounted2()));
	connect(&SdObserver2::instance(), SIGNAL(signalSdUnmounted()), this, SLOT( slot_sdUnmounted2()));

	connect(&SdObserver3::instance(), SIGNAL(signalSdMounted()), this, SLOT( slot_sdMounted3()));
	connect(&SdObserver3::instance(), SIGNAL(signalSdUnmounted()), this, SLOT( slot_sdUnmounted3()));

	m_thread1.startObserver();
	m_thread2.startObserver("two");
	m_thread3.startObserver("three");
	m_thread4.startObserver("four");
	m_thread5.startObserver("five");

}

MainWindow::~MainWindow()
{
	delete ui;

	SdObserver2::instance().stopObserver();
	SdObserver3::instance().stopObserver();
	m_thread1.stopObserver();
	m_thread2.stopObserver();
	m_thread3.stopObserver();
	m_thread4.stopObserver();
	m_thread5.stopObserver();
}

void MainWindow::on_pushButton_close_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_close_clicked]");
	QApplication::quit();
}

void MainWindow::on_pushButton_hello_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_hello_clicked]");

	MotorHandler::instance().test("Hello");
}

void MainWindow::on_pushButton_world_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_world_clicked]");
	MotorHandler::instance().test("World");
}

void MainWindow::on_pushButton_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_clicked]");
}

void MainWindow::on_pushButton_calc_clicked()
{
	qDebug("[UI] [MainWindow::on_pushButton_calc_clicked]");

	qDebug("primes = %d", DataConverter::countPrimes(10000000));
}




void MainWindow::slot_sdMounted()
{
	qDebug("[UI] [MainWindow::slot_sdMounted]");
}

void MainWindow::slot_sdUnmounted()
{
	qDebug("[UI] [MainWindow::slot_sdUnMounted]");
}


void MainWindow::slot_sdMounted2()
{
	qDebug("[UI] [MainWindow::slot_sdMounted2");
	Notification::instance().notify("mounted2");
}

void MainWindow::slot_sdUnmounted2()
{
	qDebug("[UI] [MainWindow::slot_sdUnMounted2]");
	Notification::instance().notify("unmounted2");
}

void MainWindow::slot_sdMounted3()
{
	qDebug("[UI] [MainWindow::slot_sdMounted3");
	Notification::instance().notify("mounted3");
}

void MainWindow::slot_sdUnmounted3()
{
	qDebug("[UI] [MainWindow::slot_sdUnMounted3]");
	Notification::instance().notify("unmounted3");

}

