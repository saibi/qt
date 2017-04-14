#ifndef DIALOG2_H
#define DIALOG2_H

#include "dialog.h"

class Dialog2 : public Dialog
{
	Q_OBJECT

public:
	explicit Dialog2(QWidget *parent = 0);

	virtual void setText(const QString &title);

};

#endif // DIALOG2_H
