#include "adjrule.h"

AdjRule::AdjRule()
{
	clear();
}

void AdjRule::clear()
{
	id.clear();
	title.clear();
	prerequisite.clear();
	desc.clear();
	jobType = AR_NONE;
	job.clear();
	jobOptions.clear();
	dp.clear();
	inputList.clear();
	applyList.clear();
}

bool AdjRule::isEmpty() const
{
	return (id.isEmpty() || title.isEmpty() || jobType == AR_NONE);
}

void AdjRule::dump() const
{
	if ( isEmpty() )
	{
		qDebug("Empty rule");
		return;
	}

	qDebug("* rule id [%s], title [%s]", qPrintable(id), qPrintable(title));

	for ( int i = 0 ; i < prerequisite.size(); ++i )
		qDebug("  prerequisite : %s", qPrintable(prerequisite.at(i)));

	qDebug("desc [%s]", qPrintable(desc));

	qDebug("job [%s] type-%d", qPrintable(job), jobType);

	if ( !jobOptions.isEmpty() )
		qDebug("==========\n%s\n==========", qPrintable(jobOptions));

	dp.dump();

	qDebug("input list count %d", inputList.size());
	for ( int i = 0 ; i < inputList.size(); ++i )
		inputList.at(i).dump();

	qDebug("apply list count %d", applyList.size());
	for ( int i = 0 ; i < applyList.size(); ++i )
		applyList.at(i).dump();
}

AR_DigitalPatternData::AR_DigitalPatternData()
{
	clear();
}

void AR_DigitalPatternData::clear()
{
	size = 0.0;
	width = 0.0;
	height = 0.0;
	rotate = 0.0;
}

bool AR_DigitalPatternData::isEmpty() const
{
	return rotate == 0.0 && size == 0.0 && width == 0.0 && height == 0.0;
}

void AR_DigitalPatternData::dump() const
{
	if ( !isEmpty() )
		qDebug("dp size %.2f, width %.2f, height %.2f, rotate %.2f", size, width, height, rotate);
}

AR_InputData::AR_InputData()
{
	clear();
}

void AR_InputData::clear()
{
	id.clear();
	type = AR_NONE;
	min = 0.0;
	max = 0.0;
	step = 0.0;
	defaultValue = 0.0;
}

bool AR_InputData::isEmpty() const
{
	return (id.isEmpty() || type == AR_NONE);
}

void AR_InputData::dump() const
{
	if ( !isEmpty() )
		qDebug("input id [%s] type [%d] min %.2f, max %.2f, default %.2f, step %.2f",
			   qPrintable(id), type, min, max, defaultValue, step );
}

AR_ApplyMethod::AR_ApplyMethod()
{
	clear();
}

void AR_ApplyMethod::clear()
{
	src.clear();
	target.clear();
	op = AR_NONE;
}

bool AR_ApplyMethod::isEmpty() const
{
	return (src.isEmpty() || target.isEmpty() || op == AR_NONE);
}

void AR_ApplyMethod::dump() const
{
	if ( !isEmpty() )
		qDebug("apply [%s] -> [%s], type %d", qPrintable(src), qPrintable(target), op );
}
