#ifndef DIRIMAGEWIDGET_H
#define DIRIMAGEWIDGET_H

#include <QDir>
#include "imagewidget.h"

class DirImageWidget : public ImageWidget
{
	Q_OBJECT

public:
	DirImageWidget(const QString & dirPath = "");
	~DirImageWidget();

	void setDirPath(const QString & dirPath);


protected:
	void randomPick();

	virtual void mousePressEvent(QMouseEvent *event);

private:
	QDir *m_dir;
};

#endif // DIRIMAGEWIDGET_H
