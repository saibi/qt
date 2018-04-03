#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionExit_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);
	close();
}

void MainWindow::on_actionOpen_triggered()
{
	qDebug("[%s]", Q_FUNC_INFO);

	// select file
	QString filePath = QFileDialog::getOpenFileName(this, "Open xml File", m_prevFile, "xml files (*.xml);;All files (*.*)");

	if ( filePath.isEmpty() )
	{
		qDebug("[%s] no file selected. canceled.", Q_FUNC_INFO);
		return;
	}
	qDebug("[%s] open %s.", Q_FUNC_INFO, qPrintable(filePath));





	QFile xmlFile(filePath);

	if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::critical(this,"Load XML File Problem",
			"Couldn't open xml file to load settings for download",
			QMessageBox::Ok);
			return;
	}

	QXmlStreamReader xmlReader(&xmlFile);


	//Parse the XML until we reach end of it
	while(!xmlReader.atEnd() && !xmlReader.hasError()) {
			// Read next element
			QXmlStreamReader::TokenType token = xmlReader.readNext();

			qDebug() << xmlReader.name();

			//If token is just StartDocument - go to next
			if(token == QXmlStreamReader::StartDocument) {
					continue;
			}
			//If token is StartElement - read it
			if(token == QXmlStreamReader::StartElement) {

					if(xmlReader.name() == "name") {
							continue;
					}

					if(xmlReader.name() == "id") {
						qDebug() << xmlReader.readElementText();
					}
			}
	}

	if(xmlReader.hasError()) {
			QMessageBox::critical(this,
			"xmlFile.xml Parse Error",xmlReader.errorString(),
			QMessageBox::Ok);
			return;
	}

	//close reader and flush file
	xmlReader.clear();
	xmlFile.close();

}
