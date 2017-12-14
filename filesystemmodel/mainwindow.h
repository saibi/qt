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

private:
	Ui::MainWindow *ui;

	QFileSystemModel *m_dirModel;

	QString m_cwd;
};

#endif // MAINWINDOW_H
