#include "dirimagewidget.h"
#include <QDebug>
#include <QDateTime>

DirImageWidget::DirImageWidget(const QString & dirPath) : ImageWidget()
{
	qsrand( QDateTime::currentMSecsSinceEpoch() );

	m_dir = new QDir();
	Q_CHECK_PTR(m_dir);

	setDirPath(dirPath);
}

DirImageWidget::~DirImageWidget()
{
	delete m_dir;
}


void DirImageWidget::setDirPath(const QString &dirPath)
{
	if ( dirPath.isEmpty() )
		return;

	m_dir->setPath(dirPath);
	randomPick();
}


void DirImageWidget::randomPick()
{
	static QString prevPath;

	m_dir->refresh();

	QStringList filter;
	filter << "*.png" << "*.jpg";

	QStringList files = m_dir->entryList(filter);
	if ( files.size() > 0 )
	{
		while (1)
		{
			const QString & path = files.at(qrand() % files.size());
			if ( path.compare(prevPath) != 0 )
			{
				prevPath = path;
				break;
			}
		}

		setPath( m_dir->absoluteFilePath(prevPath));
	}
}

void DirImageWidget::mousePressEvent(QMouseEvent *event)
{
	randomPick();
	ImageWidget::mousePressEvent(event);
}
