#ifndef _FIND_H_
#define _FIND_H_

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class FindFileDialog : public QDialog 
{
	Q_OBJECT
public:
	FindFileDialog(QWidget *parent = 0);

private:
	QLabel *namedLabel;
	QLineEdit *namedLineEdit;
}

#endif
