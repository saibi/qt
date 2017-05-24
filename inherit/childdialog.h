#ifndef CHILDDIALOG_H
#define CHILDDIALOG_H

#include "dialog.h"

namespace Ui {
class ChildDialog;
}

class ChildDialog : public Dialog
{
	Q_OBJECT

public:
	explicit ChildDialog(QWidget *parent = 0);
	~ChildDialog();

protected slots:
	virtual void slot_pushButton_hello_clicked();

private:
	Ui::ChildDialog *ui;
};

#endif // CHILDDIALOG_H
