#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "profilebox.h"

#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	scene = new QGraphicsScene(this);
	ui->graphicsView->setScene(scene);

	connect(scene, &QGraphicsScene::selectionChanged, this, &MainWindow::selectionChanged);

#if 0
	QBrush greenBrush(Qt::green);
	QBrush blueBrush(Qt::blue);
	QPen pen(Qt::black);
	pen.setWidth(2);

	QGraphicsRectItem * rectangle = scene->addRect(80, 0, 80, 80, pen, greenBrush);
	rectangle->setFlag(QGraphicsItem::ItemIsMovable);
	rectangle->setFlag(QGraphicsItem::ItemIsSelectable);

	QGraphicsEllipseItem * ellipse = scene->addEllipse(0, -80, 200, 60, pen, blueBrush);
	ellipse->setFlag(QGraphicsItem::ItemIsMovable);
	ellipse->setFlag(QGraphicsItem::ItemIsSelectable);

	QGraphicsTextItem * text = scene->addText("Hello World!", QFont("Times", 25));
	text->setFlag(QGraphicsItem::ItemIsMovable);
	text->setFlag(QGraphicsItem::ItemIsSelectable);
#endif

	ProfileBox * box = new ProfileBox();
	box->init("John Doe", this, scene);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::selectionChanged()
{
	qDebug() << "Item selected";
}

void MainWindow::on_pushButton_add_clicked()
{
	bool ok;
	QString name = QInputDialog::getText(this, tr("Employee Name"), tr("Please insert employee's full name here:"), QLineEdit::Normal, "John Doe", &ok);
	if ( ok && !name.isEmpty() )
	{
		ProfileBox * box = new ProfileBox();
		box->init(name, this, scene);
	}
}
