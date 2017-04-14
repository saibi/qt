#ifndef DATACONVERTER_H
#define DATACONVERTER_H

#include <QObject>
#include <QPolygonF>

class DataConverter : public QObject
{
	Q_OBJECT
public:
	explicit DataConverter(QObject *parent = 0);

	static void dumpPolygon(const QPolygonF &points);
	static bool digitalPattern_Scale(QPolygonF &points, qreal width, qreal height);
	static bool digitalPattern_ScaleOneSide(QPolygonF &points, qreal value, int direction);
	static bool digitalPattern_Rotate(QPolygonF &points, qreal angle);

	enum directions
	{
		DP_ONE_SIDE_SCALE_UP,
		DP_ONE_SIDE_SCALE_DOWN,
		DP_ONE_SIDE_SCALE_LEFT,
		DP_ONE_SIDE_SCALE_RIGHT,
	};

	static void makeCirclePolygon(QPolygonF &points, qreal radius);

	static int countPrimes(int n);

signals:

public slots:

};

#endif // DATACONVERTER_H
