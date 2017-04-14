#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QEvent>
#include <QMoveEvent>
#include <QDebug>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

protected:
	bool eventFilter(QObject *obj, QEvent *event)
	{
		if ( event->type() == QEvent::Move )
		{
			QMoveEvent * moveEvent = static_cast<QMoveEvent*>(event);
			qDebug() << "DLG Move event:" << moveEvent->pos();
		}
		else
		{
			qDebug() << "DLG Event type:" << event->type();
		}
		return QDialog::eventFilter(obj, event);
	}


private:
	Ui::Dialog *ui;
};

#endif // DIALOG_H
