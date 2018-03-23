#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}


struct FullConfigData
{
	QString section;
	QString id;
	QString value;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void dumpDb(const QString & filePath);

private slots:
	void on_action_Open_triggered();

	void on_actionE_xit_triggered();

protected:

	bool loadConfigDBAll(const QString & path, QList<FullConfigData> & dataList);

private:
	Ui::MainWindow *ui;

	QString m_prevFile;
};

#endif // MAINWINDOW_H
