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

AR_DigitalPatternData::AR_DigitalPatternData()
{
	clear();
}

void AR_DigitalPatternData::clear()
{
	rotate = 0.0;
	size = 0.0;
}

bool AR_DigitalPatternData::isEmpty() const
{
	return rotate == 0.0 && size == 0.0;
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
