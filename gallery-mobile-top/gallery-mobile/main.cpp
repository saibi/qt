#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "albummodel.h"
#include "picturemodel.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	AlbumModel albumModel;
	PictureModel pictureModel(albumModel);

	QQmlApplicationEngine engine;

	QQmlContext * context = engine.rootContext();
	context->setContextProperty("albumModel", &albumModel);
	context->setContextProperty("pictureModel", &pictureModel);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
