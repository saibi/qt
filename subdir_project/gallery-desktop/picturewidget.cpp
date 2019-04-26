#include "picturewidget.h"
#include "ui_picturewidget.h"

#include "picturemodel.h"
#include "thumbnailproxymodel.h"

PictureWidget::PictureWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PictureWidget),
	mModel(nullptr),
	mSelectionModel(nullptr)
{
	ui->setupUi(this);

	ui->pictureLabel->setMinimumSize(1,1);

	connect(ui->backButton, SIGNAL(clicked()), this, SIGNAL(backToGallery()));
	connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deletePicture()));
	connect(ui->previousButton, &QPushButton::clicked, [this] () {
		QModelIndex currentModelIndex = mSelectionModel->currentIndex();
		QModelIndex previousModelIndex = mSelectionModel->model()->index(currentModelIndex.row() - 1, 0);
		mSelectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
	} );
	connect(ui->nextButton, &QPushButton::clicked, [this] () {
		QModelIndex currentModelIndex = mSelectionModel->currentIndex();
		QModelIndex nextModelIndex = mSelectionModel->model()->index(currentModelIndex.row() + 1, 0);
		mSelectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
	} );
}

PictureWidget::~PictureWidget()
{
	delete ui;
}

void PictureWidget::setModel(ThumbnailProxyModel *model)
{
	mModel = model;
}

void PictureWidget::setSelectionModel(QItemSelectionModel *selectionModel)
{
	mSelectionModel = selectionModel;
	if ( !mSelectionModel )
		return;
	connect(mSelectionModel, &QItemSelectionModel::selectionChanged, this, &PictureWidget::loadPicture);
}

void PictureWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	updatePicturePixmap();
}

void PictureWidget::updatePicturePixmap()
{
	if ( mPixmap.isNull() )
		return;

	ui->pictureLabel->setPixmap(mPixmap.scaled(ui->pictureLabel->size(), Qt::KeepAspectRatio));
}

void PictureWidget::deletePicture()
{
	int row = mSelectionModel->currentIndex().row();
	mModel->removeRow(mSelectionModel->currentIndex().row());

	QModelIndex previousModelIndex = mModel->index(row - 1, 0);
	if ( previousModelIndex.isValid() )
	{
		mSelectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
		return;
	}

	QModelIndex nextModelIndex = mModel->index(row, 0);
	if ( nextModelIndex.isValid() )
	{
		mSelectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
		return;
	}

	emit backToGallery();
}

void PictureWidget::loadPicture(const QItemSelection &selected)
{
	if ( selected.indexes().isEmpty() ) {
		ui->nameLabel->setText("");
		ui->pictureLabel->setPixmap(QPixmap());
		ui->deleteButton->setEnabled(false);
		return;
	}

	QModelIndex current = selected.indexes().at(0);
	mPixmap = QPixmap(mModel->data(current, PictureModel::Roles::FilePathRole).toString());

	ui->nameLabel->setText(mModel->data(current, Qt::DisplayRole).toString());
	updatePicturePixmap();

	ui->previousButton->setEnabled(current.row() > 0 );
	ui->nextButton->setEnabled(current.row() < (mModel->rowCount() -1));
	ui->deleteButton->setEnabled(true);
}
