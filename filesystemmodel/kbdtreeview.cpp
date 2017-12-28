#include "kbdtreeview.h"


KbdTreeView::KbdTreeView(QWidget *parent) : QTreeView(parent)
{

}

void KbdTreeView::keyPressEvent(QKeyEvent *e)
{
	QTreeView::keyPressEvent(e);

	emit keyPressed(e->key());
}
