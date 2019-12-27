#ifndef DATETIMEDECORATORTESTS_H
#define DATETIMEDECORATORTESTS_H

#include <QtTest>
#include <data/datetime-decorator.h>
#include <test-suite.h>

namespace cm {

namespace data {


class DateTimeDecoratorTests : public TestSuite
{
	Q_OBJECT

public:
	DateTimeDecoratorTests();

private slots:
	void constructor_givenNoParameters_setsDefaultProperties();
	void constructor_givenParameters_setsProperties();

	void setValue_givenNewValue_updateValueAndEmitsSignal();
	void setValue_givenSameValue_takesNoAction();

	void jsonValue_whenDefaultValues_returnsJson();
	void jsonValue_whenValueSet_returnsJson();

	void update_whenPresentInJson_updateValue();
	void update_whenNotPresentInJson_updatesValueToDefault();

	void toIso8601String_whenDefaultValue_returnsString();
	void toIso8601String_whenValueSet_returnsString();
	void toPrettyDateString_whenDefaultValue_returnsString();
	void toPrettyDateString_whenValueSet_returnsString();
	void toPrettyTimeString_whenDefaultValue_returnsString();
	void toPrettyTimeString_whenValueSet_returnsString();
	void toPrettyString_whenDefaultValue_returnsString();
	void toPrettyString_whenValueSet_returnsString();

private:
	QDateTime testDate { QDate(2019, 12, 27), QTime(16, 40, 00) };
};
}}


#endif // DATETIMEDECORATORTESTS_H
