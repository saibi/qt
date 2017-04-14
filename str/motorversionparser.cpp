#include "motorversionparser.h"

MotorVersionParser::MotorVersionParser()
{
}

bool MotorVersionParser::setVerStr(QString ver)
{
	if ( ver.isEmpty() )
		return false;

	QString verStr = ver.section("_", 0, 0 );
	QString dateStr = ver.section("_",1, 1 );
	QString buildStr = ver.section("_", 2, 2);


}

