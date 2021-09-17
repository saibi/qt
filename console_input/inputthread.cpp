#include "inputthread.h"
#include <QTextStream>

InputThread::InputThread(QObject * parent) : QThread(parent),
	m_stopFlag(false)
{

}

void InputThread::run()
{

	QTextStream stream(stdin);
	QString line;

	qDebug("[InputThread::run] start");

	while ( !m_stopFlag )
	{
		line = stream.readLine();

		emit signal_line(line);
		qDebug("[InputThread::run] [%s]", qPrintable(line));

		if (line.compare("quit") == 0 )
			break;
	}
	qDebug("[InputThread::run] end");

}

