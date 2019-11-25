#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	manager = new QNetworkAccessManager(this);

	ftpAddress = "ftp://127.0.0.1/";
	ftpPort = 21;

	username = "saibi";
	password = "rladudals";

	getFileList();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::getFileList()
{
	QUrl ftpPath;

	ftpPath.setUrl(ftpAddress + "/home/saibi/files.txt");
	ftpPath.setUserName(username);
	ftpPath.setPassword(password);
	ftpPath.setPort(ftpPort);

	QNetworkRequest request;
	request.setUrl(ftpPath);


	downloadFileListReply = manager->get(request);
	connect(downloadFileListReply, &QNetworkReply::finished, this, &MainWindow::downloadFileListFinished);
}


void MainWindow::downloadFileListFinished()
{
	if (downloadFileListReply->error() != QNetworkReply::NoError )
	{
		QMessageBox::warning(this, "Failed", "Failed to load file list: " + downloadFileListReply->errorString());
	}
	else
	{
		QByteArray responseData;
		if (downloadFileListReply->isReadable() )
		{
			responseData = downloadFileListReply->readAll();

		}
		ui->listWidget->clear();

		fileList = QString(responseData).split(",");

		if ( fileList.size() > 0 )
		{
			for ( int i = 0; i < fileList.size(); ++i )
			{
				if (fileList.at(i) != "" )
				{
					ui->listWidget->addItem(fileList.at(i));

				}
			}
		}
	}
}

void MainWindow::on_pushButton_open_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select File", qApp->applicationDirPath());
	ui->lineEdit_upload->setText(fileName);
}

void MainWindow::on_pushButton_upload_clicked()
{
	QFile * file = new QFile(ui->lineEdit_upload->text());
	QFileInfo fileInfo(*file);

	uploadFileName = fileInfo.fileName();

	QUrl ftpPath;
	ftpPath.setUrl(ftpAddress + "/home/saibi/" + uploadFileName);
	ftpPath.setUserName(username);
	ftpPath.setPassword(password);
	ftpPath.setPort(ftpPort);

	if ( file->open(QIODevice::ReadOnly) )
	{
		ui->progressBar_upload->setEnabled(true);
		ui->progressBar_upload->setValue(0);

		QNetworkRequest request;
		request.setUrl(ftpPath);

		uploadFileReply = manager->put(request, file);
		connect(uploadFileReply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadFileProgress(qint64, qint64)));
		connect(uploadFileReply, SIGNAL(finished()), this, SLOT(uploadFileFinished()));
	}
	else
	{
		QMessageBox::warning(this, "Invalid File", "Failed to open file for uplaod.");
	}
}


void MainWindow::uploadFileProgress(qint64 bytesSent, qint64 bytesTotal)
{
	if ( bytesTotal > 0 )
	{
		qint64 percentage = 100 * bytesSent / bytesTotal;
		ui->progressBar_upload->setValue((int) percentage );
	}
}


void MainWindow::uploadFileFinished()
{
	if (uploadFileReply->error() != QNetworkReply::NoError )
	{
		QMessageBox::warning(this, "Failed", "Failed to upload file: " + uploadFileReply->errorString());
	}
	else
	{
		bool exists = false;

		if ( fileList.size() > 0 )
		{
			for ( int i = 0 ;  i < fileList.size(); ++i )
			{
				if ( fileList.at(i) == uploadFileName )
				{
					exists = true;
				}
			}
		}

		if ( !exists )
		{
			fileList.append(uploadFileName);

			QString fileName = "files.txt";
			QFile * file = new QFile(qApp->applicationDirPath() + "/" + fileName);

			file->open(QIODevice::ReadWrite);
			if ( fileList.size() > 0 )
			{
				for (int j = 0 ; j < fileList.size(); j++ )
				{
					if ( fileList.at(j) != "" )
					{
						file->write(QString(fileList.at(j) + ",").toUtf8());
					}
				}
			}
			file->close();


			QFile *newFile = new QFile(qApp->applicationDirPath() + "/" + fileName);

			if ( newFile->open(QIODevice::ReadOnly) )
			{
				QUrl ftpPath;
				ftpPath.setUrl(ftpAddress + "/home/saibi/" + fileName);
				ftpPath.setUserName(username);
				ftpPath.setPassword(password);
				ftpPath.setPort(ftpPort);

				QNetworkRequest request;
				request.setUrl(ftpPath);
				uploadFileListReply = manager->put(request, newFile);
				connect(uploadFileListReply, SIGNAL(finished()), this, SLOT(uploadFileListFinished()));
				file->close();
			}
		}

		QMessageBox::information(this, "Success", "File successfully uploaded.");

	}
}


void MainWindow::uploadFileListFinished()
{
	if ( uploadFileListReply->error() != QNetworkReply::NoError )
	{
		QMessageBox::warning(this, "Failed", "Failed to update file list: " + uploadFileListReply->errorString());
	}
	else
	{
		getFileList();
	}
}

void MainWindow::on_pushButton_folder_clicked()
{
	QString folder = QFileDialog::getExistingDirectory(this, "Open Directory", qApp->applicationDirPath(), QFileDialog::ShowDirsOnly);
	ui->lineEdit_download->setText(folder);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	downloadFileName = item->text();

	QString folder = ui->lineEdit_download->text();

	if ( folder != "" && QDir(folder).exists() )
	{
		QUrl ftpPath;

		ftpPath.setUrl(ftpAddress + "/home/saibi/" + downloadFileName);
		ftpPath.setUserName(username);
		ftpPath.setPassword(password);
		ftpPath.setPort(ftpPort);

		QNetworkRequest request;
		request.setUrl(ftpPath);

		downloadFileReply = manager->get(request);
		connect(downloadFileReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadFileProgress(qint64, qint64)));
		connect(downloadFileReply, SIGNAL(finished()), this, SLOT(downloadFileFinished()));
	}
	else
	{
		QMessageBox::warning(this, "Invalid Path", "Please set the download path before download.");
	}
}

void MainWindow::downloadFileProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if ( bytesTotal > 0 )
	{
		qint64 percentage = 100 * bytesReceived/ bytesTotal;
		ui->progressBar_download->setValue((int)percentage);
	}
}


void MainWindow::downloadFileFinished()
{
	if ( downloadFileReply->error() != QNetworkReply::NoError )
	{
		QMessageBox::warning(this, "Failed", "Failed to download file: " + downloadFileReply->errorString() );
	}
	else
	{
		QByteArray responseData;
		if ( downloadFileReply->isReadable() )
		{
			responseData = downloadFileReply->readAll();
		}

		if ( !responseData.isEmpty() )
		{
			QString folder = ui->lineEdit_download->text();
			QFile file(folder + "/" + downloadFileName);
			file.open(QIODevice::WriteOnly);
			file.write(responseData);
			file.close();

			QMessageBox::information(this, "Success", "File successfully downloaded.");

		}
	}
}
