#include "mythread.h"
#include "imageviewer.h"
#include "camera.h"
#include "led.h"
#include <QDebug>

MyThread::MyThread()
{
	m_stopFlag = false;
}

void MyThread::run()
{
	int index;

	m_stopFlag = false;

	qDebug() << Q_FUNC_INFO << " start MyThread " << currentThread();

	init_led();
	open_device();
    init_device();
    start_capturing();

	while( m_stopFlag == false )
	{
    	index = mainloop();
		if ( index >= 0 )
			emit updateThread(index);
    }

	stop_capturing();
    uninit_device();
    close_device();
	close_led();
	qDebug() << Q_FUNC_INFO << " end ";
}
