#include "dlg_step.h"
#include "ui_dlg_step.h"

Dlg_Step::Dlg_Step(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dlg_Step)
{
	ui->setupUi(this);
}

Dlg_Step::~Dlg_Step()
{
	delete ui;
}

void Dlg_Step::on_pushButton_2_clicked()
{
	this->accept();
}

void Dlg_Step::showEvent(QShowEvent *)
{
	ui->widget_ing->hide();
	adjustSize();
}

void Dlg_Step::on_pushButton_clicked()
{
	if ( ui->widget_ing->isVisible())
		ui->widget_ing->hide();
	else
		ui->widget_ing->show();

	ui->widget_2->adjustSize();
	adjustSize();
}

void Dlg_Step::on_pushButton_4_clicked()
{
	adjustSize();
	update();
}
