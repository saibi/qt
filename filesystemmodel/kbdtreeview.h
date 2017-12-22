#ifndef KBDTREEVIEW_H
#define KBDTREEVIEW_H

#include <QTreeView>
#include <QKeyEvent>

class KbdTreeView : public QTreeView
{

	Q_OBJECT

public:
	explicit KbdTreeView(QWidget *parent = 0);


protected:
	void keyPressEvent(QKeyEvent *e);

signals:
	void keyPressed(int key);
};

#endif // KBDTREEVIEW_H
