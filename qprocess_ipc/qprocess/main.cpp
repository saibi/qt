#include <QCoreApplication>

#include <QProcess>
#include <QDebug>
#include <QObject>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QProcess * childProcess = new QProcess(0);

	QObject::connect(childProcess, &QProcess::readyReadStandardOutput, [childProcess] {
		qDebug().noquote() << "[*]" << childProcess->readAll();

	});


	QObject::connect(childProcess, &QProcess::started, [childProcess] {
		childProcess->write("saibi\n");
	});

	childProcess->start("/tmp/helloname"); // hello binary path

	return a.exec();
}
