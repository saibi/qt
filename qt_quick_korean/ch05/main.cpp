#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include "dataobject.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QList<QObject *> dataList;

	dataList.append( new DataObject("Item 1", "red"));
	dataList.append( new DataObject("Item 2", "green"));
	dataList.append( new DataObject("Item 3", "blue"));
	dataList.append( new DataObject("Item 4", "yellow"));

	QQuickView view;

	view.setResizeMode(QQuickView::SizeRootObjectToView);
	QQmlContext *ctxt = view.rootContext();

	ctxt->setContextProperty("MyModel", QVariant::fromValue(dataList));

	view.setSource(QUrl("qrc:/view.qml"));
	view.show();

	return app.exec();
}
