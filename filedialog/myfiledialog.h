#ifndef MYFILEDIALOG_H
#define MYFILEDIALOG_H

#include <QFileDialog>

class MyFileDialog : public QFileDialog
{
	Q_OBJECT

public:
	MyFileDialog(QWidget * parent, Qt::WindowFlags flags);
	MyFileDialog(QWidget * parent = 0, const QString & caption = QString(), const QString & directory = QString(), const QString & filter = QString());
};

#endif // MYFILEDIALOG_H
