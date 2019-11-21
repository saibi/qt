#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQuickItem>
#include <QQuickView>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QObject * target = qobject_cast<QObject *>(ui->quickWidget->rootObject());

	QMetaObject::invokeMethod(target, "addMarker", Qt::AutoConnection,
							  Q_ARG(QVariant, "HUVITZ"), Q_ARG(QVariant, 37.399061), Q_ARG(QVariant, 126.965953));

}

MainWindow::~MainWindow()
{
	delete ui;
}
