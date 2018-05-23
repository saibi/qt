#include "a20cameracalibthread.h"
#include "a20disp.h"

#ifdef __linux__
#include <sys/ioctl.h>
#include <errno.h>
#endif

A20CameraCalibThread::A20CameraCalibThread(QObject *parent) : QThread(parent)
{
	m_running = false;
	m_stopFlag = false;
	m_camWidth = 0;
	m_camHeight = 0;
	m_reqCount = 0;
	m_dispCount = 0;
	for(int i = 0 ; i < 256 ; ++i)
		m_yuv_rgb_conv_color_tab.append(qRgb(i,i,i));

	m_bCompensation = true;
}

void A20CameraCalibThread::stop()
{
	if ( m_running )
	{
		qDebug("[%s] stopped", Q_FUNC_INFO);
		m_stopFlag = true;
		wait();
	}
}

void A20CameraCalibThread::startThread(int camWidth, int camHeight, QVector<QVector<QPointF> > &calibrationPosition)
{
	if ( m_running )
	{
		qDebug("[%s] already started", Q_FUNC_INFO);
		return;
	}

	QMutexLocker locker(&m_mutex);

	m_camWidth = camWidth;
	m_camHeight = camHeight;
	m_calibrationPosition = calibrationPosition;
	m_reqCount = 0;
	m_dispCount = 0;
	m_stopFlag = false;
	m_tmpImg = QImage(m_camWidth, m_camHeight, QImage::Format_Indexed8);
	m_tmpImg.setColorTable(m_yuv_rgb_conv_color_tab);

	m_waitList.clear();
	m_dispList.clear();
	m_doneList.clear();


#ifdef PUFF_DUMP_IMAGE
	m_imgList.clear();
#endif
	start(QThread::NormalPriority);
}

bool A20CameraCalibThread::calibNDisp(struct CameraDisplayData *p)
{
	if ( !m_running )
	{
		return false;
	}

	++m_reqCount;
	if ( m_mutex.tryLock() )
	{
		if ( m_waitList.isEmpty() )
		{
			//qDebug("DBG append %p, idx %d, %s", p->camAddr, p->camBuffer.index, qPrintable(QTime::currentTime().toString("hh:mm:ss zzz")) );

			m_waitList.append(*p);
			m_mutex.unlock();
			return true;
		}
		m_mutex.unlock();
	}
	return false;
}

void A20CameraCalibThread::run()
{
#ifdef __arm_A20__
	m_running = true;

	qDebug("[%s] loop start", Q_FUNC_INFO);
	while (!m_stopFlag)
	{

#ifdef PUFF_DUMP_IMAGE
		if ( !m_imgList.isEmpty() )
		{
			m_mutex.lock();
			const struct DumpItem & item = m_imgList.takeFirst();
			m_mutex.unlock();

			QString path = "/dump/cam" + item.time.toString("hhmmss_zzz") + ".png"; // QString("_%1.png").arg(item.no, 4, 10, QChar('0'));
			item.img.save(path);
			qDebug("DBG save [%s]", qPrintable(path));
		}
#else
		{
			QMutexLocker locker(&m_mutex);

			if ( !m_waitList.isEmpty() )
			{
				const struct CameraDisplayData & item = m_waitList.takeFirst();

				if ( *(item.camAddr+0) == 0xc8 && *(item.camAddr+1) == 0xc8)
				{
					qDebug("DBG cam image err %p , idx %d, %s", item.camAddr, item.camBuffer.index, qPrintable(QTime::currentTime().toString("hh:mm:ss zzz")));
					for (int i = 0 ; i < m_dispList.size() ; ++i )
						qDebug("DBG     disp %p , idx %d", m_dispList.at(i).camAddr, m_dispList.at(i).camBuffer.index);
					for (int i = 0 ; i < m_doneList.size() ; ++i )
						qDebug("DBG done %p , idx %d", m_doneList.at(i).camAddr, m_doneList.at(i).camBuffer.index);

					m_doneList.append(item);
					continue;
				}

				calibrateImage(item.camAddr, m_tmpImg);
				//qDebug("DBG calib %p , idx %d, %s", item.camAddr, item.camBuffer.index, qPrintable(QTime::currentTime().toString("hh:mm:ss zzz")));

				if ( item.dispUpdate )
				{
					A20Disp::instance().setCamBufAddr( item.camBuffer.m.offset);
					++m_dispCount;

					if ( !m_dispList.isEmpty() )
					{
						m_doneList.append(m_dispList);
						m_dispList.clear();
					}
					m_dispList.append(item);
				}
				else
				{
					m_doneList.append(item);
				}
			}
		}
#endif // PUFF_DUMP_IMAGE

		msleep(10);
	}
	qDebug("[%s] loop end. reqCount = %d, dispCount = %d", Q_FUNC_INFO, m_reqCount, m_dispCount);

	m_running = false;

#ifdef PUFF_DUMP_IMAGE
	qDebug("DBG %d imgs remain", m_imgList.size());
	for ( int i = 0 ; i < m_imgList.size(); ++i )
	{
		const struct DumpItem & item = m_imgList.at(i);
		QString path = "/dump/cam" + item.time.toString("hhmmss_zzz") + ".png"; // QString("_%1.png").arg(item.no, 4, 10, QChar('0'));
		item.img.save(path);
		qDebug("DBG save [%s]", qPrintable(path));
	}
#else
	m_doneList.append(m_dispList);
	m_doneList.append(m_waitList);
#endif



#endif
}

void A20CameraCalibThread::calibrateImage(char* distortedImage, QImage& tempImage)
{
	if (m_bCompensation == false || m_camWidth != m_calibrationPosition.size())
		return;

	const int offset = 20;
	const int originalImageSize = m_camWidth;
	const int xCount = originalImageSize - offset;
	const int yCount = originalImageSize - offset;
	uchar* tempImageStart = tempImage.bits();

	memcpy(tempImageStart, distortedImage, tempImage.byteCount());

	for (int i = offset; i < yCount; i++)
	{
		for (int j = offset; j < xCount; j++)
		{
			QPointF& residual = m_calibrationPosition[i][j];

			float xPosition = residual.x();
			float yPosition = residual.y();

			int x1 = xPosition;
			int x2 = x1 + 1;
			int y1 = yPosition;
			int y2 = y1 + 1;

			int gray11 = *(tempImageStart + originalImageSize * y1 + x1);
			int gray21 = *(tempImageStart + originalImageSize * y1 + x2);
			int gray12 = *(tempImageStart + originalImageSize * y2 + x1);
			int gray22 = *(tempImageStart + originalImageSize * y2 + x2);

			int grayTop = (gray21 - gray11) * (xPosition - x1) + gray11;
			int grayBottom = (gray22 - gray12) * (xPosition - x1) + gray12;

			int grayLevel = (grayBottom - grayTop) * (yPosition - y1) + grayTop;

			*(distortedImage + i * originalImageSize + j) = grayLevel;
		}
	}

	// DBG img test
	*(distortedImage + 0) = 0xc8;
	*(distortedImage + 1) = 0xc8;
}

char * A20CameraCalibThread::getLastDispBuf()
{
	QMutexLocker locker(&m_mutex);

	if ( m_dispList.isEmpty() )
		return 0;
	else
		return m_dispList.last().camAddr;
}

void A20CameraCalibThread::setCalibrationData(QVector<QVector<QPointF> > &calibrationPosition)
{
	QMutexLocker locker(&m_mutex);

	m_calibrationPosition = calibrationPosition;
}

bool A20CameraCalibThread::getDoneList(QList<CameraDisplayData> &list)
{
	if ( m_mutex.tryLock() )
	{
		list = m_doneList;
		m_doneList.clear();
		m_mutex.unlock();
		return true;
	}
	return false;
}


#ifdef PUFF_DUMP_IMAGE
bool A20CameraCalibThread::dispNSave(char *camBuf, unsigned int offset)
{
	if ( !m_running )
		return false;

	A20Disp::instance().setCamBufAddr(offset);
	++m_dispCount;

	memcpy(m_tmpImg.bits(), camBuf, m_tmpImg.byteCount());

	struct DumpItem item;

	item.no = m_dispCount;
	item.time = QTime::currentTime();
	item.img = m_tmpImg;

	QMutexLocker locker(&m_mutex);
	m_imgList.append(item);
	return true;
}
#endif
