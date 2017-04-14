#include "dataconverter.h"

#include <QTransform>
#include <qmath.h>

DataConverter::DataConverter(QObject *parent) :
	QObject(parent)
{
}

void DataConverter::dumpPolygon(const QPolygonF &points)
{
	for (int i = 0 ; i < points.size() ; i ++ )
		qDebug("#%d (%.2f, %.2f)", i, points.at(i).x(), points.at(i).y());
}

bool DataConverter::digitalPattern_Scale(QPolygonF &points, qreal width, qreal height)
{
	QRectF boundingRect;
	qreal originalWidth, originalHeight;
	qreal widthScaleRate, heightScaleRate;
	int i;

	if (width <= 0.0 || height <= 0.0)
	{
		qDebug("[DataConverter::digitalPattern_Scale] Error: Width: [%f], Height: [%f]", width, height);
		return false;
	}

	int count = points.size();

	if (count < 10)
	{
		qDebug("[DataConverter::digitalPattern_Scale] Error: Polygon Point Count: [%d]", points.size());
		return false;
	}

	boundingRect = points.boundingRect();

	originalWidth = boundingRect.width();
	originalHeight = boundingRect.height();

	widthScaleRate = width / originalWidth;
	heightScaleRate = height / originalHeight;

	if (width == originalWidth && height == originalHeight)
		return true;

	for (i = 0; i < count; i ++)
	{
		points[i].setX(points.at(i).x() * widthScaleRate);
		points[i].setY(points.at(i).y() * heightScaleRate);
	}

	//qDebug("*** Width: [%f] -> [%f]", originalWidth, points.boundingRect().width());
	//qDebug("*** Height: [%f] -> [%f]", originalHeight, points.boundingRect().height());

	return true;
}



bool DataConverter::digitalPattern_ScaleOneSide(QPolygonF &points, qreal value, int direction)
{
	QRectF boundingRect;
	qreal originalvalue = 1;
	qreal scaleRate;
	int i;

	if (value <= 0.0)
	{
		qDebug("[DataConverter::digitalPattern_ScaleOneSide] Error: Value: [%f]", value);
		return false;
	}

	if (points.size() < 10)
	{
		qDebug("[DataConverter::digitalPattern_ScaleOneSide] Error: Polygon Point Count: [%d]", points.size());
		return false;
	}

	boundingRect = points.boundingRect();

	switch (direction)
	{
	case DP_ONE_SIDE_SCALE_UP:		originalvalue = boundingRect.top();		break;
	case DP_ONE_SIDE_SCALE_DOWN:	originalvalue = boundingRect.bottom();	break;
	case DP_ONE_SIDE_SCALE_LEFT:	originalvalue = boundingRect.left();	break;
	case DP_ONE_SIDE_SCALE_RIGHT:	originalvalue = boundingRect.right();	break;
	}

	originalvalue = qAbs(originalvalue);

	scaleRate = value / originalvalue;

	if (value == originalvalue)
		return true;


	switch (direction)
	{
	case DP_ONE_SIDE_SCALE_UP:
		for (i = 0; i < points.size(); i++)
		{
			if (points.at(i).y() < 0.0)
				points[i].setY(points.at(i).y() * scaleRate);
		}
		break;

	case DP_ONE_SIDE_SCALE_DOWN:
		//qDebug("##### DP DOWN DUMP #####");
		for (i = 0; i < points.size(); i++)
		{
			//qDebug("#%d (%.4f, %.4f)", i, points.at(i).x(), points.at(i).y());
			if (points.at(i).y() > 0.0)
			{
				points[i].setY(points.at(i).y() * scaleRate);
				//qDebug("==> #%d (%.4f, %.4f)", i, points.at(i).x(), points.at(i).y());
			}
		}
		break;

	case DP_ONE_SIDE_SCALE_LEFT:
		for (i = 0; i < points.size(); i++)
		{
			if (points.at(i).x() < 0.0)
				points[i].setX(points.at(i).x() * scaleRate);
		}
		break;

	case DP_ONE_SIDE_SCALE_RIGHT:
		for (i = 0; i < points.size(); i++)
		{
			if (points.at(i).x() > 0.0)
				points[i].setX(points.at(i).x() * scaleRate);
		}
		break;
	}

	//qDebug("*** Top:    [%f] -> [%f]", boundingRect.top(), points.boundingRect().top());
	//qDebug("*** Bottom: [%f] -> [%f]", boundingRect.bottom(), points.boundingRect().bottom());
	//qDebug("*** Left:   [%f] -> [%f]", boundingRect.left(), points.boundingRect().left());
	//qDebug("*** Right:  [%f] -> [%f]", boundingRect.right(), points.boundingRect().right());

	return true;
}


bool DataConverter::digitalPattern_Rotate(QPolygonF &points, qreal angle)
{
	if (angle == 0.0)
		return true;

	if (angle < -180.0 || angle > 180.0)
	{
		qDebug("[DataConverter::digitalPattern_Rotate] Error: Angle: [%f]", angle);
		return false;
	}

	if (points.size() < 10)
	{
		qDebug("[DataConverter::digitalPattern_Rotate] Error: Polygon Point Count: [%d]", points.size());
		return false;
	}

	//
	QTransform transform;

	transform.rotate(angle);

	points = transform.map(points);

	//
	int bottomIndex = 0;

	{
		qreal tmpX = -9999.99;

		for (int i = 0; i < points.size(); i++)
		{
			if (points.at(i).y() < 0.0)
				continue;

			if (points.at(i).x() > 0.0)
				continue;

			if (points.at(i).x() > tmpX)
			{
				tmpX = points.at(i).x();
				bottomIndex = i;
			}
		}
	}

	qDebug("** bottomIndex: [%d]", bottomIndex);

	if (bottomIndex > 0)
	{
		QPointF tmpPoint;

		for (int i = points.size() - 1; i >= bottomIndex; i--)
		{
			tmpPoint = points.last();
			points.pop_back();
			points.push_front(tmpPoint);
		}
	}

	return true;
}


void DataConverter::makeCirclePolygon(QPolygonF &points, qreal radius)
{
	points.clear();

	for (int i = 0; i < 1440; i++)
	{
		points.append( QPointF( radius * qSin( 2 *3.14145 * i / 1440.0), radius * qCos( 2 * 3.14159 * i / 1440.0 )) );
	}
}

int DataConverter::countPrimes(int n)
{
	unsigned int i, j;
	int countprimes = 0;

	for ( i = 1; i <= n; i++ )
	{
		bool isprime = true;

		for ( j = 2; j <= qSqrt(i); j++)
		{
			if ( ( i % j) == 0 )
			{
				isprime = false;
				break;
			}
		}

		if ( isprime ) countprimes++;

	}
	return countprimes;
}
