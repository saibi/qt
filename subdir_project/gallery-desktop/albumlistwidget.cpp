#include "albumlistwidget.h"
#include "ui_albumlistwidget.h"

#include <QInputDialog>

#include "albummodel.h"

AlbumListWidget::AlbumListWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AlbumListWidget),
	mAlbumModel(nullptr)
{
	ui->setupUi(this);

	connect(ui->createAlbumButton, SIGNAL(clicked()), this, SLOT(createAlbum()));
}

AlbumListWidget::~AlbumListWidget()
{
	delete ui;
}

void AlbumListWidget::setModel(AlbumModel *model)
{
	mAlbumModel = model;
	ui->albumList->setModel(mAlbumModel);
}

void AlbumListWidget::setSelectionModel(QItemSelectionModel *selectionModel)
{
	ui->albumList->setSelectionModel(selectionModel);
}

void AlbumListWidget::createAlbum()
{
	if ( !mAlbumModel )
		return;

	bool ok;

	QString albumName = QInputDialog::getText(this, "Create a new Album", "Choose a name", QLineEdit::Normal, "New album", &ok);

	if ( ok && !albumName.isEmpty() )
	{
		Album album(albumName);
		QModelIndex createdIndex = mAlbumModel->addAlbum(album);
		ui->albumList->setCurrentIndex(createdIndex);
	}
}
