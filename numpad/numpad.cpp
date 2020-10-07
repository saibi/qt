#include <QDebug>
#include "numpad.h"
#include "ui_numpad.h"

numpad::numpad(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::numpad)
{
	ui->setupUi(this);
}

numpad::~numpad()
{
	delete ui;
}

void numpad::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void numpad::on_btn_clicked()
{

	QPushButton * sender = qobject_cast<QPushButton *>(QObject::sender());

	qDebug() << sender->text() << " Btn clicked.";
}
