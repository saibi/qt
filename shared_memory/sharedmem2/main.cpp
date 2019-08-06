#include <QCoreApplication>

#include <QDebug>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QSharedMemory * sharedMemory = new QSharedMemory("mykey", 0);
	sharedMemory->attach();
	sharedMemory->lock();

	QByteArray sharedData(sharedMemory->size(), '\0');

	memcpy(sharedData.data(), sharedMemory->data(), sharedMemory->size());
	sharedMemory->unlock();

	QString sharedMessage = QString::fromUtf8(sharedData);
	qDebug() << sharedMessage;

	sharedMemory->detach();
	return a.exec();
}
