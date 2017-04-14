#include "cmdthread.h"
#include <QFile>
#include <QCryptographicHash>
#include <QDebug>

CmdThread::CmdThread(QObject *parent) :
	QThread(parent)
{
	m_progress = 0;
	m_id = -1;
	m_error = 0;
}

void CmdThread::startCommand(int id)
{
	if ( id < 0 || id >= CTID_MAX )
	{
		m_id = -1;
	}
	else
	{
		m_id = id;
		start();
	}
}

int CmdThread::getProgress()
{
	return m_progress;
}

int CmdThread::getError()
{
	return m_error;
}

void CmdThread::setArgs(const QList <QString> &args)
{
	m_args = args;
}

void CmdThread::run()
{
	switch(m_id )
	{
	case CTID_TEST:
		testCommand();
		break;

	default:
		qDebug("%s: Invalid command ID", Q_FUNC_INFO);
		return;
	}
}

void CmdThread::testCommand()
{
	m_progress = 1;
	m_error = 0;

	for ( int i = 0 ; i < m_args.count(); i++ )
	{
		qDebug("arg #%d : %s", i+1, m_args.at(i).toLocal8Bit().constData());
	}

	QFile file("/Users/saibi/test.mp4");
	QByteArray md5 = QCryptographicHash::hash( file.readAll(), QCryptographicHash::Md5);
	qDebug() << md5.toHex();

	m_progress = 50;
	qDebug("remove");
	QFile::remove("/Users/saibi/test.bak");
	qDebug("copy");
	QFile::copy("/Users/saibi/test.mp4", "/Users/saibi/test.bak");
	qDebug("completed");
	m_progress = 100;
}
