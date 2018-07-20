#include "esawidget.h"
#include "ui_esawidget.h"

#include "esaxmlreader.h"

ESAWidget::ESAWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ESAWidget)
{
	ui->setupUi(this);

	EsaXmlReader esa;

	esa.read(":/esa.xml");
}

ESAWidget::~ESAWidget()
{
	delete ui;
}
