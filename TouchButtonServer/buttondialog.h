#ifndef BUTTONDIALOG_H
#define BUTTONDIALOG_H

#include <QDialog>

namespace Ui {
class ButtonDialog;
}

class ButtonDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ButtonDialog(QWidget *parent = 0);
	~ButtonDialog();

	void setId(int id) { m_id = id; }

private slots:
	void on_pushButton_pressed();

	void on_pushButton_released();

private:
	Ui::ButtonDialog *ui;

	int m_id;
};

#endif // BUTTONDIALOG_H
