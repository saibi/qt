#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
	void updateContextBtn();


private slots:
	void on_btn_file_open_clicked();

	void on_btn_extract_clicked();

	void on_btn_exit_clicked();

	void on_btn_open_work_directory_clicked();

	void on_btn_change_context_clicked();

	void on_radioButton_fixed_clicked();

	void on_radioButton_file_clicked();

	void on_radioButton_section_clicked();

private:
    Ui::MainWindow *ui;

	QStringList	m_file_list;
	QString m_context;
};

#endif // MAINWINDOW_H
