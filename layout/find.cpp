#include "find.h"

FindFileDialog::FindFileDialog(QWidget *parent) : QDialog(parent)
{
        namedLabel = new QLabel(tr("Named :"));
	namedLineEdit = new QLineEdit;
        namedLabel->setBuddy(namedLineEdit);

	namedLabel->setGeometry(9, 9, 50, 25);
	namedLineEdit->setGeometry(65, 9, 200, 25);

	setWindowTitle(tr("Find Files or Folders"));
	setFixedSize(365, 240);
}
