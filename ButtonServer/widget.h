#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLocalServer>

#include "buttonwidget.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

protected:
	virtual void showEvent(QShowEvent *);

signals:
	void signalCommandReceived(const QString &cmdStr);

private slots:
	void on_pushButton_clicked();
	void handleConnection();
	void slotReadCommand();
	void slotParseCommand(const QString & cmdStr);

private:
	Ui::Widget *ui;

	QLocalServer *server;
	QLocalSocket *socket;

	int m_count;

	QList< QPoint > m_prevPosList;

	ButtonWidget *m_button;
	int m_buttonId;
};

#endif // WIDGET_H
