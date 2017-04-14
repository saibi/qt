#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <qlocalsocket.h>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	m_count = 0;

	socket = 0;
	server = new QLocalServer(this);
	if (!server->listen("ButtonServer")) {
		QMessageBox::critical(this, tr("Button Server"),
							  tr("Unable to start the server: %1.")
							  .arg(server->errorString()));
		close();
		return;
	}

	connect(server, SIGNAL(newConnection()), this, SLOT(handleConnection()));
	connect(this, SIGNAL(signalCommandReceived(QString)), this, SLOT(slotParseCommand(QString)));

	m_prevPosList.append(QPoint(100, 100));
	m_prevPosList.append(QPoint(200, 200));

	m_button = new ButtonWidget();
	m_buttonId = 0;
}

Widget::~Widget()
{
	delete server;
	delete ui;
}

void Widget::showEvent(QShowEvent *)
{
	m_button->show();
	move(m_prevPosList.at(0));
	m_button->move(m_prevPosList.at(1));
}

void Widget::on_pushButton_clicked()
{
	++m_count;

	qDebug("[Widget::on_pushButton_clicked()] %d", m_count);

	if ( m_count > 10 )
	{
		m_button->close();
		close();
	}
}

void Widget::slotReadCommand()
{
	qDebug("[Widget::slotReadCommand()]");

	if ( socket )
	{
		connect(socket, SIGNAL(disconnected()),
				socket, SLOT(deleteLater()));

		QDataStream in(socket);
		in.setVersion(QDataStream::Qt_4_7);

		qDebug("[Widget::slotReadCommand()] bytesAvailable = %d", socket->bytesAvailable());

		QString cmd;

		in >> cmd;

		qDebug("[Widget::slotReadCommand()] cmd = [%s]", qPrintable(cmd));
		socket = 0;

		emit signalCommandReceived(cmd);
	}
}

void Widget::handleConnection()
{
	qDebug("[Widget::handleConnection()]");

	socket = server->nextPendingConnection();

	if ( socket )
		connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadCommand()));
}

void Widget::slotParseCommand(const QString &cmdStr)
{
	QString cmd = cmdStr.section(' ', 0, 0);
	QWidget *p = this;

	if ( m_buttonId == 1 )
		p = m_button;
	else
		p = this;

	qDebug("[Widget::slotParseCommand] cmd = [%s]", qPrintable(cmd));

	if ( cmd.compare("move", Qt::CaseInsensitive) == 0 )
	{
		QString param = cmdStr.section(' ', 1, 1);
		if ( param.compare("right", Qt::CaseInsensitive) == 0 )
		{
			p->move( p->pos().x() + 100, p->pos().y() );
			qDebug("Move Right");
		}
		else if ( param.compare("left", Qt::CaseInsensitive) == 0 )
		{
			p->move( p->pos().x() - 100, p->pos().y() );
			qDebug("Move Left");
		}
		else if ( param.compare("up", Qt::CaseInsensitive) == 0 )
		{
			p->move( p->pos().x(), p->pos().y() - 100 );
			qDebug("Move Up");
		}
		else if ( param.compare("down", Qt::CaseInsensitive) == 0 )
		{
			p->move( p->pos().x(), p->pos().y() + 100 );
			qDebug("Move Down");
		}
		else
		{
			param = cmdStr.section(' ', 1);

			QString x = param.section(',', 0, 0);
			QString y = param.section(',', 1, 1);

			p->move( x.toInt(), y.toInt());

			qDebug("Move (%d, %d)", x.toInt(), y.toInt());


		}
	}
	else if ( cmd.compare("show", Qt::CaseInsensitive) == 0 )
	{
		qDebug("Show");
		p->show();
		p->move(m_prevPosList.at(m_buttonId));
	}
	else if ( cmd.compare("hide", Qt::CaseInsensitive) == 0 )
	{
		qDebug("Hide");
		m_prevPosList[m_buttonId] = p->pos();
		p->hide();
	}
	else if ( cmd.compare("id", Qt::CaseInsensitive) == 0 )
	{
		QString idStr = cmdStr.section(' ', 1, 1);

		m_buttonId = idStr.toInt();

		if ( m_buttonId < 0 || m_buttonId > 1 )
			m_buttonId = 0;

		qDebug("Button ID = %d", m_buttonId);
	}
}
