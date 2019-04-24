#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	mTasks()
{
	ui->setupUi(this);

	connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::slotAddTask);

	updateStatus();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slotAddTask()
{
	bool ok;

	QString name = QInputDialog::getText(this, tr("Add task"), tr("Task name"), QLineEdit::Normal, tr("Untitled task"), &ok);

	if ( ok && !name.isEmpty() )
	{
		qDebug() << "Adding new task";

		Task * task = new Task(name);
		connect(task, &Task::signalRemoved, this, &MainWindow::slotRemoveTask);
		connect(task, SIGNAL(signalStatusChanged(Task*)), this, SLOT(slotTaskStatusChanged(Task*)));
		mTasks.append(task);
		ui->taskLayout->addWidget(task);
		updateStatus();
	}
}

void MainWindow::slotRemoveTask(Task *task)
{
	mTasks.removeOne(task);
	ui->taskLayout->removeWidget(task);
	task->setParent(0);
	delete task;
	updateStatus();
}

void MainWindow::slotTaskStatusChanged(Task *)
{
	updateStatus();
}

void MainWindow::updateStatus()
{
	int completedCount = 0;

	for ( auto t : mTasks)
	{
		if ( t->isCompleted() )
			completedCount++;
	}

	int todoCount = mTasks.size() - completedCount;

	ui->statusLabel->setText(QString("Status: %1 todo / %2 completed").arg(todoCount).arg(completedCount));
}
