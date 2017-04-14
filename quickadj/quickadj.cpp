#include "quickadj.h"
#include "ui_quickadj.h"

QuickAdj::QuickAdj(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::QuickAdj)
{
	ui->setupUi(this);
}

QuickAdj::~QuickAdj()
{
	delete ui;
}
