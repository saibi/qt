#ifndef PROFILEBOX_H
#define PROFILEBOX_H

#include "mainwindow.h"

class MainWindow;

class ProfileBox : public QGraphicsItemGroup
{
public:
	explicit ProfileBox(QGraphicsItemGroup * parent = nullptr);

	void init(QString name, MainWindow * window, QGraphicsScene * scene);

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
	QGraphicsTextItem * nameTag;

	MainWindow * mainWindow;
};

#endif // PROFILEBOX_H
