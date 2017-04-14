#include "buttondialog.h"
#include "ui_buttondialog.h"

ButtonDialog::ButtonDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ButtonDialog)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

	m_id = 0;
}

ButtonDialog::~ButtonDialog()
{
	delete ui;
}

void ButtonDialog::on_pushButton_pressed()
{
	qDebug("[ButtonDialog::on_pushButton_pressed] m_id = %d", m_id);
}

void ButtonDialog::on_pushButton_released()
{
	qDebug("[ButtonDialog::on_pushButton_released] m_id = %d", m_id);
}
