#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	startTimer(1000);

	QThread * m_thread = new QThread;
	Worker * m_worker = new Worker("hello");
	m_worker->moveToThread(m_thread);

	connect(m_thread, SIGNAL(started()), m_worker, SLOT(process()));
	connect(m_worker, SIGNAL(finished()), m_thread, SLOT(quit()));
	connect(m_worker, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
	connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
	connect(m_worker, SIGNAL(finished()), this, SLOT(slot_workerFinished()));
	connect(m_worker, SIGNAL(progress(QString, unsigned int)), this, SLOT(slot_workerProgress(QString, unsigned int)));

	m_thread->start();

	{
		QThread * thread = new QThread;
		Worker * worker = new Worker("world");
		worker->moveToThread(thread);

		connect(thread, SIGNAL(started()), worker, SLOT(process()));
		connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
		connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(worker, SIGNAL(finished()), this, SLOT(slot_workerFinished()));
		connect(worker, SIGNAL(progress(QString, unsigned int)), this, SLOT(slot_workerProgress(QString, unsigned int)));

		thread->start();
	}

	{
		QThread * thread = new QThread;
		Worker * worker = new Worker("HI");
		worker->moveToThread(thread);

		connect(thread, SIGNAL(started()), worker, SLOT(process()));
		connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
		connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(worker, SIGNAL(finished()), this, SLOT(slot_workerFinished()));
		connect(worker, SIGNAL(progress(QString, unsigned int)), this, SLOT(slot_workerProgress(QString, unsigned int)));

		thread->start();
	}

	{
		QThread * thread = new QThread;
		Worker * worker = new Worker("Good");
		worker->moveToThread(thread);

		connect(thread, SIGNAL(started()), worker, SLOT(process()));
		connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
		connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(worker, SIGNAL(finished()), this, SLOT(slot_workerFinished()));
		connect(worker, SIGNAL(progress(QString, unsigned int)), this, SLOT(slot_workerProgress(QString, unsigned int)));

		thread->start();
	}

	{
		QThread * thread = new QThread;
		Worker * worker = new Worker("bad");
		worker->moveToThread(thread);

		connect(thread, SIGNAL(started()), worker, SLOT(process()));
		connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
		connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		connect(worker, SIGNAL(finished()), this, SLOT(slot_workerFinished()));
		connect(worker, SIGNAL(progress(QString, unsigned int)), this, SLOT(slot_workerProgress(QString, unsigned int)));

		thread->start();
	}

	ui->widgetWait->show();
	ui->widgetWait->setAnimationDelay(35);
	ui->widgetWait->startAnimation();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_startWorker_clicked()
{
	qDebug("[MainWindow::on_pushButton_startWorker_clicked]");

}

void MainWindow::slot_workerFinished()
{
	qDebug("[MainWindow::slot_workerFinished]");
}

void MainWindow::slot_workerProgress(QString name, unsigned int count)
{
	qDebug("[MainWindow::slot_workerProgress] [%s] %d", name.toLocal8Bit().constData(), count);
}

void MainWindow::timerEvent(QTimerEvent *)
{
	static unsigned int count = 0;
	qDebug("[MainWindow::timerEvent] %d", count++);
}
