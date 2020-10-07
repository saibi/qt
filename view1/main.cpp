#include <QtGui>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QSplitter *splitter = new QSplitter;
	QDirModel *model = new QDirModel;
	QTreeView *tree = new QTreeView(splitter);
	tree->setModel(model);
	tree->setRootIndex(model->index(QDir::currentPath()));

	QListView *list = new QListView(splitter);
	list->setModel(model);
	list->setRootIndex(model->index(QDir::currentPath()));

	splitter->setWindowTitle("Two views onto the same directory model");
	splitter->show();
	return app.exec();
}

