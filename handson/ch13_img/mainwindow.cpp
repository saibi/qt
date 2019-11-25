#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	imageBuffer = nullptr;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Image File", qApp->applicationDirPath(), "JPG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)");

	if ( !fileName.isEmpty() )
	{
		imageBuffer = new QPixmap(fileName);
		update();
	}
}


void MainWindow::resizeImage()
{
	if ( imageBuffer != nullptr)
	{
		QSize size = ui->label_image->size();
		QPixmap pixmap = imageBuffer->scaled(size, Qt::KeepAspectRatio);

		QRect rect = ui->label_image->rect();
		rect.setX((this->size().width() - pixmap.width()) / 2);
		rect.setY((this->size().height() - pixmap.height()) / 2 );

		QPainter painter;

		painter.begin(this);
		painter.drawPixmap(rect, pixmap, ui->label_image->rect());
		painter.end();
	}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	resizeImage();
}
