#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QCryptographicHash>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(&m_thread, SIGNAL(finished()), this, SLOT(cmdFinished()));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	QList <QString> args;
	m_dlgProgress.setTime(ui->lineEdit->text().toInt());
	m_dlgProgress.show();

	qDebug("before");
	args.append(QString("test"));
	args.append(QString("tost"));
	m_thread.setArgs(args);
	m_thread.startCommand(CmdThread::CTID_TEST);
	qDebug("after");
}

void MainWindow::cmdFinished()
{
	qDebug("thread finished");
	m_dlgProgress.terminate();
}
