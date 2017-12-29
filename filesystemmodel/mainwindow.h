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

	void handle_up();
	bool handle_match();
	void handle_select(const QModelIndex &index);
	bool handle_delete();
	void handle_copy(bool cutMode);
	void handle_paste();
	void handle_rename();
	void handle_create();

private slots:
	void on_treeView_doubleClicked(const QModelIndex &index);

	void on_pushButton_exit_clicked();

	void on_pushButton_match_clicked();

	void on_pushButton_up_clicked();

	void slot_keyPressed(int key);

	void on_actionExit_triggered();


	void on_actionUp_triggered();

	void on_actionMatch_triggered();

	void on_action_Delete_triggered();

	void on_action_Copy_triggered();

	void on_actionCut_X_triggered();

	void on_action_Paste_triggered();



	void on_action_Rename_triggered();

	void on_action_New_Directory_triggered();

private:
	Ui::MainWindow *ui;

	QFileSystemModel *m_dirModel;

	QString m_cwd;
	QList <QString> m_clipFileList;
	QString m_clipDir;
	bool m_cutMode;
};

#endif // MAINWINDOW_H
