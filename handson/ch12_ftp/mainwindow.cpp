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
	ui->uploadFileInput->setText(fileName);
}
