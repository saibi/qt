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
	static QString getFileMD5(QString path);
	static bool saveMD5(QString path, QString md5);

private:
	void addFileList(void);

private slots:
	void on_btn_file_open_clicked();

	void on_btn_exit_clicked();

	void on_btn_open_work_directory_clicked();

	void on_btn_generate_clicked();

private:
    Ui::MainWindow *ui;

	QStringList		m_file_list;
};

#endif // MAINWINDOW_H
