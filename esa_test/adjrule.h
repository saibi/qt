#ifndef ADJRULE_H
#define ADJRULE_H

#include <QString>
#include <QList>
#include <QStringList>

class AR_DigitalPatternData
{
public:
	AR_DigitalPatternData();
	void clear();
	bool isEmpty() const;
	void dump() const;

	float size;
	float width;
	float height;

	float rotate;
};

class AR_InputData
{
public:
	AR_InputData();
	void clear();
	bool isEmpty() const;
	void dump() const;

	QString id;
	int type;
	float min;
	float max;
	float step;
	float defaultValue;
};

class AR_ApplyMethod
{
public:
	AR_ApplyMethod();
	void clear();
	bool isEmpty() const;
	void dump() const;

	QString src;
	QString target;
	int op;
};


class AdjRule
{
public:
	AdjRule();
	void clear();
	bool isEmpty() const;
	void dump() const;

	QString id;
	QString title;
	QStringList prerequisite;
	QString desc;
	int jobType;
	QString job;
	QString jobOptions;
	AR_DigitalPatternData dp;
	QList <AR_InputData> inputList;
	QList <AR_ApplyMethod> applyList;
};

enum AdjRuleConstants
{
	AR_NONE = 0,
	AR_JOB_INTERNAL,
	AR_JOB_FILE,
	AR_INPUT_FLOAT,
	AR_INPUT_INT,
	AR_OP_SET,
	AR_OP_ADD,
};


#endif // ADJRULE_H
