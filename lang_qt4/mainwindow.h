#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    void retranslateUi(void);

    QTranslator tr_ko;
    QTranslator tr_de;

private slots:
    void on_actionEnglish_triggered();
    void on_actionDeutsch_triggered();
    void on_actionKorean_triggered();
};

#endif // MAINWINDOW_H
