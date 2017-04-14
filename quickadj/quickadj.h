#ifndef QUICKADJ_H
#define QUICKADJ_H

#include <QDialog>

namespace Ui {
class QuickAdj;
}

class QuickAdj : public QDialog
{
	Q_OBJECT

public:
	explicit QuickAdj(QWidget *parent = 0);
	~QuickAdj();

private:
	Ui::QuickAdj *ui;
};

#endif // QUICKADJ_H
