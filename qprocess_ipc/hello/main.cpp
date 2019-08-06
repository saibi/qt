#include <QCoreApplication>

#include <QTextStream>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);


	QTextStream out(stdout);
	QTextStream in(stdin);

	out << QString("Please enter your name:\n");
	out.flush();

	QString name = in.readLine();
	out << "Hello " << name << "\n";

	return 0;
}
