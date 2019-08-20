#include <QCoreApplication>
#include "httprequest.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	HttpRequest req;

	req.executeGet();
	req.executePost();
	req.executeBlockingGet();

	return a.exec();
}
