#include <QCoreApplication>

#include <QSharedMemory>

#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QString sharedMessage("Hello");
	QByteArray sharedData = sharedMessage.toUtf8();

	QSharedMemory * sharedMemory = new QSharedMemory("mykey", 0);
	sharedMemory->create(sharedData.size());
	sharedMemory->lock();
	memcpy(sharedMemory->data(), sharedData.data(), sharedData.size());
	sharedMemory->unlock();

	qDebug() << "memory shared";

	return a.exec();
}
