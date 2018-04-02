#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDropEvent>

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
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);

	int readDumpTextFile(const QString & file, QList <QByteArray> & dumpList);

	QByteArray textDumpToBin(const QByteArray & src);

	void convertScreenShot(const QString & filePath);

private slots:
	void on_actionExit_triggered();

	void on_actionOpen_triggered();

private:
	Ui::MainWindow *ui;

	QString m_prevFile;


	QVector <QRgb> m_colorTable;

};

#endif // MAINWINDOW_H
