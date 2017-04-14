#include "noti.h"

noti::noti()
{
	m_label = new QLabel();
}

noti::~noti()
{
}

void noti::notify(const QString &message, int notifySecond, bool refreshNow)
{
	m_label->setText(message);
	m_label->show();
	m_label->update();
}
