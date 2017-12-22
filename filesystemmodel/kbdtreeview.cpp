#include "kbdtreeview.h"


KbdTreeView::KbdTreeView(QWidget *parent) : QTreeView(parent)
{

}

void KbdTreeView::keyPressEvent(QKeyEvent *e)
{
	qDebug("[%s] %d", Q_FUNC_INFO, e->key());
	QTreeView::keyPressEvent(e);

	emit keyPressed(e->key());
}
