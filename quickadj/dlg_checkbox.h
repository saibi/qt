#ifndef DLG_CHECKBOX_H
#define DLG_CHECKBOX_H

#include <QDialog>

namespace Ui {
class Dlg_Checkbox;
}

class Dlg_Checkbox : public QDialog
{
	Q_OBJECT

public:
	explicit Dlg_Checkbox(QWidget *parent = 0);
	~Dlg_Checkbox();

private:
	Ui::Dlg_Checkbox *ui;
};

#endif // DLG_CHECKBOX_H
