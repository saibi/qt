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

	auto prettyName = [] (const QString & taskName) -> QString {
		return "-------- " + taskName.toUpper();
	};
	connect(ui->removeButton, &QPushButton::clicked, [this, name, prettyName] {
		qDebug() << "Trying to remove" << name << prettyName(name);
		this->emit signalRemoved(this);
	} );

	connect(ui->checkBox, SIGNAL(toggled(bool)), this, SLOT(slotChecked(bool)));
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

void Task::slotChecked(bool checked)
{
	QFont font(ui->checkBox->font());

	font.setStrikeOut(checked);
	ui->checkBox->setFont(font);
	emit signalStatusChanged(this);
}
