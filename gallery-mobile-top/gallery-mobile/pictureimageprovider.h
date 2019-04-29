#ifndef PICTUREIMAGEPROVIDER_H
#define PICTUREIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QCache>

class PictureModel;

class PictureImageProvider : public QQuickImageProvider
{
public:
	PictureImageProvider(PictureModel * pictureModel);

	QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

	static const QSize THUMBNAIL_SIZE;

	QPixmap* pictureFromCache(const QString& filePath, const QString& pictureSize);

private:
	PictureModel * mPictureModel;
	QCache <QString, QPixmap> mPicturesCache;

};

#endif // PICTUREIMAGEPROVIDER_H
