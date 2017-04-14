#include "notificationwidget.h"
#include "ui_notificationwidget.h"

#include <QDesktopWidget>

NotificationWidget::NotificationWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::NotificationWidget)
{
	ui->setupUi(this);

	setWindowFlags(
				Qt::Tool |
				Qt::FramelessWindowHint |
				Qt::WindowSystemMenuHint |
				Qt::WindowStaysOnTopHint
				);
	setAttribute(Qt::WA_TranslucentBackground);
}

NotificationWidget::~NotificationWidget()
{
	delete ui;
}

void NotificationWidget::showEvent(QShowEvent *)
{
	//qDebug("[NotificationWidget::showEvent]");

	QRect frect = this->frameGeometry();
	frect.moveCenter(
				QPoint(
					QDesktopWidget().availableGeometry().center().x(),
					QDesktopWidget().availableGeometry().bottom() - (this->height() / 1.5)
					)
				);

	this->move(frect.topLeft());
}

void NotificationWidget::mousePressEvent(QMouseEvent *)
{
	//qDebug("[NotificationWidget::mousePressEvent]");
}

void NotificationWidget::setText(const QString &text)
{
	ui->label->setText(text);
}

QString NotificationWidget::text()
{
	return ui->label->text();
}

//void NotificationWidget::on_pushButton_close_clicked()
//{
//	this->hide();
//}
