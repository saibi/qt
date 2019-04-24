#include "task.h"
#include "ui_task.h"
#include <QInputDialog>
#include <QDebug>

Task::Task(const QString & name, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Task)
{
	ui->setupUi(this);
	setName(name);

	connect(ui->editButton, SIGNAL(clicked()), this, SLOT(slotRename()));
	connect(ui->removeButton, &QPushButton::clicked, [this, name] {
		qDebug() << "Trying to remove" << name;
		this->emit signalRemoved(this);
	} );
}

Task::~Task()
{
	delete ui;
}

void Task::setName(const QString &name)
{
	ui->checkBox->setText(name);
}

QString Task::name() const
{
	return ui->checkBox->text();
}

bool Task::isCompleted() const
{
	return ui->checkBox->isChecked();
}


void Task::slotRename()
{
	bool ok;

	QString value = QInputDialog::getText(this, tr("Edit task"), tr("Task name"), QLineEdit::Normal, this->name(), &ok);

	if ( ok && !value.isEmpty() )
		setName(value);

}
