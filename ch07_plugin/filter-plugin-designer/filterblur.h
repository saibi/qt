#ifndef FILTERBLUR_H
#define FILTERBLUR_H

#include "filter.h"

class FilterBlur : public Filter
{
public:
	FilterBlur();
	~FilterBlur();

	QImage process(const QImage &image) override;

};

#endif // FILTERBLUR_H
