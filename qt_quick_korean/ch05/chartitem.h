#ifndef CHARTITEM_H
#define CHARTITEM_H

#include <QQuickPaintedItem>

class BarItem;

class ChartItem : public QQuickPaintedItem
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<BarItem> bars READ bars NOTIFY barsChanged)

public:
	ChartItem(QQuickItem *parent = nullptr);

	void paint(QPainter * painter);

	QQmlListProperty<BarItem> bars();

signals:
	void barsChanged();

private:
	static void append_bar(QQmlListProperty<BarItem> * list, BarItem *bar);
	QList<BarItem*> m_bars;
};

#endif // CHARTITEM_H
