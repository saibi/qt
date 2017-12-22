#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
	void on_treeView_doubleClicked(const QModelIndex &index);

	void on_pushButton_exit_clicked();

	void on_pushButton_rename_clicked();

	void on_pushButton_up_clicked();

	void slot_keyPressed(int key);

private:
	Ui::MainWindow *ui;

	QFileSystemModel *m_dirModel;

	QString m_cwd;
};

#endif // MAINWINDOW_H
