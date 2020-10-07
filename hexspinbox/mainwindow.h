#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QLineEdit>

#include "hexspinbox.h"

namespace Ui
{
        class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

private:
        Ui::MainWindow *ui;

        HexSpinBox spinBox;
        QLabel *namedLabel;
        QLineEdit *namedLineEdit;
};

#endif // MAINWINDOW_H
