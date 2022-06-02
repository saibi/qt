#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QLabel>
class ImageWidget : public QWidget
{
	Q_OBJECT

public:
	ImageWidget(const QString & path = "", QWidget *parent = 0);
	void setPath(const QString & path);

private:
	QLabel * m_label;
};

#endif // IMAGEWIDGET_H
