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
	QList <AdjRule> ruleList;

	int ret = esa.read(":/esa.xml", ruleList);
	if ( ret > 0 )
	{
		qDebug("DBG %d rules", ruleList.size());
		for( int i = 0 ; i < ruleList.size(); ++i )
			ruleList.at(i).dump();
	}
}

ESAWidget::~ESAWidget()
{
	delete ui;
}
