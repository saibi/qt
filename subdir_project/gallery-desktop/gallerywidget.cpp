#include "gallerywidget.h"
#include "ui_gallerywidget.h"


#include <QItemSelectionModel>


GalleryWidget::GalleryWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GalleryWidget)
{
	ui->setupUi(this);
	ui->albumListWidget->setMaximumWidth(250);

	connect(ui->albumWidget, SIGNAL(pictureActivated(QModelIndex)), this, SIGNAL(pictureActivated(QModelIndex)));
}

GalleryWidget::~GalleryWidget()
{
	delete ui;
}


void GalleryWidget::setAlbumModel(AlbumModel *albumModel)
{
	ui->albumListWidget->setModel(albumModel);
	ui->albumWidget->setAlbumModel(albumModel);
}

void GalleryWidget::setAlbumSelectionModel(QItemSelectionModel *albumSelectionModel)
{
	ui->albumListWidget->setSelectionModel(albumSelectionModel);
	ui->albumWidget->setAlbumSelectionModel(albumSelectionModel);
}


void GalleryWidget::setPictureModel(ThumbnailProxyModel *pictureModel)
{
	ui->albumWidget->setPictureModel(pictureModel);
}

void GalleryWidget::setPictureSelectionModel(QItemSelectionModel *pictureSelectionModel)
{
	ui->albumWidget->setPictureSelectionModel(pictureSelectionModel);
}
