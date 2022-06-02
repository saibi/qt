#include "imagewidget.h"

#include <QVBoxLayout>

ImageWidget::ImageWidget(const QString & path, QWidget *parent) : QWidget(parent)
{
	m_label = new QLabel(this);
	Q_CHECK_PTR(m_label);
	m_label->setScaledContents(true);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(m_label);
	layout->setMargin(0);
	this->setLayout(layout);

	setPath(path);
}

void ImageWidget::setPath(const QString &path)
{
	if ( !path.isEmpty() )
	{
		QPixmap pixmap;

		pixmap.load(path);
		m_label->setPixmap(pixmap);
	}
}
