#ifndef FILTERORIGINAL_H
#define FILTERORIGINAL_H

#include "filter.h"

class FilterOriginal : public Filter
{
public:
	FilterOriginal();
	~FilterOriginal();

	QImage process(const QImage& image) override;
};


#endif // FILTERORIGINAL_H
