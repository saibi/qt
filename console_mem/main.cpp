#include <QCoreApplication>

#include <QDebug>

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



	return a.exec();
}
