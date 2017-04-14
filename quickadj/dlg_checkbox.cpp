#include "dlg_checkbox.h"
#include "ui_dlg_checkbox.h"

Dlg_Checkbox::Dlg_Checkbox(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dlg_Checkbox)
{
	ui->setupUi(this);
}

Dlg_Checkbox::~Dlg_Checkbox()
{
	delete ui;
}
