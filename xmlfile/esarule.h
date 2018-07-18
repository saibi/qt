#ifndef ESARULE_H
#define ESARULE_H

#include <QString>
#include <QList>
#include <QByteArray>

class EsaRule
{
public:
	EsaRule();

	int id;
	QString title;
	QList <int> prerequisite;
	QString desc;
	QString job_name;
	int job_type;
	enum JobTypeIndex
	{
		INTERNAL_DATA = 0,
		FILE = 1,
	};

	QByteArray job_option;

	float dp_rotate;
	float dp_size;




};

#endif // ESARULE_H
