#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void getFileList();

private slots:
	void downloadFileListFinished();
	void uploadFileProgress(qint64 bytesSent, qint64 bytesTotal);
	void uploadFileFinished();
	void uploadFileListFinished();
	void downloadFileProgress(qint64 bytesReceived, qint64 bytesTotal);
	void downloadFileFinished();

	void on_pushButton_open_clicked();

	void on_pushButton_upload_clicked();

	void on_pushButton_folder_clicked();

	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
	Ui::MainWindow *ui;

	QNetworkAccessManager * manager;

	QString ftpAddress;
	int ftpPort;
	QString username;
	QString password;

	QNetworkReply * downloadFileListReply;
	QNetworkReply * uploadFileListReply;

	QNetworkReply * uploadFileReply;
	QNetworkReply * downloadFileReply;

	QStringList fileList;
	QString uploadFileName;
	QString downloadFileName;


};

#endif // MAINWINDOW_H
