#include "esawidget.h"
#include "ui_esawidget.h"

#include "esaxmlreader.h"

ESAWidget::ESAWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ESAWidget)
{
	ui->setupUi(this);

//	setWindowFlags(Qt::FramelessWindowHint);
//	setAttribute(Qt::WA_NoSystemBackground);
//	setAttribute(Qt::WA_TranslucentBackground);
//	setAttribute(Qt::WA_TransparentForMouseEvents);

	setWindowOpacity(0.8);

	EsaXmlReader esa;

	esa.read(":/esa.xml");
}

ESAWidget::~ESAWidget()
{
	delete ui;
}
