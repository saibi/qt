#ifndef DLG_STEP_H
#define DLG_STEP_H

#include <QDialog>

namespace Ui {
class Dlg_Step;
}

class Dlg_Step : public QDialog
{
	Q_OBJECT

public:
	explicit Dlg_Step(QWidget *parent = 0);
	~Dlg_Step();

protected:
	void showEvent(QShowEvent *);

private slots:
	void on_pushButton_2_clicked();

	void on_pushButton_clicked();

	void on_pushButton_4_clicked();

private:
	Ui::Dlg_Step *ui;
};

#endif // DLG_STEP_H
