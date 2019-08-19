#include <QCoreApplication>

#include <QDebug>
#include <QDateTime>

#include <QFile>
#include <QTextStream>

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString & message)
{

	QString levelText;

	switch(type) {
	case QtDebugMsg:
		levelText = "Debug";
		break;

	case QtInfoMsg:
		levelText = "Info";
		break;

	case QtWarningMsg:
		levelText = "Warning";
		break;

	case QtCriticalMsg:
		levelText = "Critical";
		break;

	case QtFatalMsg:
		levelText = "Fatal";
		break;

	}

	QString text = QString("[%1] %2").arg(levelText).arg(message);

	QFile file("/tmp/log_test.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream textStream(&file);
	textStream << text << endl;
}

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

	qInstallMessageHandler(messageHandler);


	qsrand(QDateTime::currentDateTime().toTime_t());
	qDebug() << qrand() % 10 << qrand() % 10;

	Person person = { "Lenna", 64 };

	qDebug() << "Person info" << person;


	qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss}] [%{type}] %{function} %{message}");

	qInfo() << "a.exec...";



	return a.exec();
}
