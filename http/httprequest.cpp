#include "httprequest.h"


HttpRequest::HttpRequest(QObject *parent) :
	QObject(parent),
	mAccessManager()
{
	connect(&mAccessManager, &QNetworkAccessManager::finished, this, &HttpRequest::replyFinished);
}


void HttpRequest::executeGet()
{
	QNetworkRequest request(QUrl("http://httpbin.org/ip"));
	mAccessManager.get(QNetworkRequest(request));
}

void HttpRequest::replyFinished(QNetworkReply *reply)
{
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	qDebug() << "Response network error" << reply->error();
	qDebug() << "Response HTTP status code" << statusCode;
	qDebug() << "Reply content:" << reply->readAll();

	reply->deleteLater();
}

#include <QUrlQuery>

void HttpRequest::executePost()
{
	QNetworkRequest request(QUrl("http://httpbin.org/post"));

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QUrlQuery urlQuery;
	urlQuery.addQueryItem("book", "Mastering Qt 5");

	QUrl params;
	params.setQuery(urlQuery);

	QNetworkReply * reply = mAccessManager.post( request, params.toEncoded());

	connect(reply, &QNetworkReply::readyRead, [reply] () {
		qDebug() << "Ready to read from reply";
	});

	connect(reply, &QNetworkReply::sslErrors, [this] (QList<QSslError> errors) {
		qWarning() << "SSL errors" << errors;
	});
}

#include <QEventLoop>

void HttpRequest::executeBlockingGet()
{
	QNetworkAccessManager localManager;

	QEventLoop eventLoop;

	QObject::connect(&localManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);

	QNetworkRequest request(QUrl("http://httpbin.org/user-agent"));
	request.setHeader(QNetworkRequest::UserAgentHeader, "MasteringQt5Browser 1.0");

	QNetworkReply * reply = localManager.get(request);
	eventLoop.exec();

	qDebug() << "Blocking GET result:" << reply->readAll();

	reply->deleteLater();
}
