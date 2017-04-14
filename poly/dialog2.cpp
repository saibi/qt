#include "dialog2.h"
#include "ui_dialog.h"

Dialog2::Dialog2(QWidget *parent) :
	Dialog(parent)
{
}

void Dialog2::setText(const QString &title)
{
	ui->label->setText("dialog2 " + title);
}
