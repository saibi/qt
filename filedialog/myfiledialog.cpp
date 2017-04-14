#include "myfiledialog.h"

MyFileDialog::MyFileDialog(QWidget * parent, Qt::WindowFlags flags) : QFileDialog(parent, flags)
{
}

MyFileDialog::MyFileDialog(QWidget * parent, const QString & caption, const QString & directory, const QString & filter) : QFileDialog(parent, caption, directory, filter)
{
}


