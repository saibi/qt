#include <QCoreApplication>

#include <QDebug>
#include <QDateTime>


struct Person {
	QString name;
	int age;
};


QDebug operator<< (QDebug debug, const Person & person)
{
	QDebugStateSaver saver(debug);

	debug.nospace() << "(" << "name: " << person.name << ", " << "age: " << person.age << ")";
	return debug;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	bool boolean = true;
	int integer = 5;
	char character = 'A';
	int * integerPointer = &integer;


	qDebug() << "boolean is:" << boolean;
	qDebug() << "integer is:" << integer;
	qDebug() << "character is:" << character;
	qDebug() << "intergerpointer is:" << integerPointer;
	qDebug() << "*integerpointer is:" << *integerPointer;
	qDebug() << "done!";

	qsrand(QDateTime::currentDateTime().toTime_t());
	qDebug() << qrand() % 10 << qrand() % 10;


	Person person = { "Lenna", 64 };

	qDebug() << "Person info" << person;

	return a.exec();
}
