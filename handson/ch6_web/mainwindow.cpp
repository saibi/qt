#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	webview = new QWebEngineView(ui->webviewFrame);
	loadPage();

	connect(webview, SIGNAL(loadProgress(int)), this, SLOT(webviewLoading(int)));
	connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(webviewLoaded()));


	QWebChannel * channel = new QWebChannel(this);
	channel->registerObject("mainwindow", this);
	webview->page()->setWebChannel(channel);


	webview->load(QUrl("qrc:///html/hello.html"));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_back_clicked()
{
	webview->back();
}

void MainWindow::on_pushButton_forward_clicked()
{
	webview->forward();
}

void MainWindow::on_pushButton_refresh_clicked()
{
	webview->reload();
}

void MainWindow::on_pushButton_go_clicked()
{
	loadPage();
}

void MainWindow::loadPage()
{
	QString url = ui->lineEdit->text();
	if ( !url.startsWith("http://") && !url.startsWith("https://") )
	{
		url = "http://" + url;
	}
	ui->lineEdit->setText(url);
	webview->load(QUrl(url));
}

void MainWindow::on_lineEdit_returnPressed()
{
	loadPage();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QMainWindow::paintEvent(event);
	webview->resize(ui->webviewFrame->size());
}

void MainWindow::webviewLoading(int progress)
{
	ui->progressBar->setValue(progress);
}

void MainWindow::webviewLoaded()
{
	ui->lineEdit->setText(webview->url().toString());
}

#include <QFileDialog>

void MainWindow::on_pushButton_pdf_clicked()
{
	QString path = QFileDialog::getSaveFileName(this, "enter filename");

	if ( !path.isEmpty() )
		webview->page()->printToPdf(path);
}

void MainWindow::on_pushButton_hello_clicked()
{
	webview->page()->runJavaScript("hello()");
}

void MainWindow::doSomething()
{
	ui->label->setText("java script");
}
