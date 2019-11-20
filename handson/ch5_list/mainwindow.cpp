#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->listWidget->addItem("My Test Item");

	QListWidgetItem * listItem = new QListWidgetItem;
	listItem->setText("My Second Item");
	listItem->setData(100, 1000);
	ui->listWidget->addItem(listItem);

	qDebug() << listItem->data(100);


	QTreeWidgetItem * treeItem = new QTreeWidgetItem;
	treeItem->setText(0, "My Test Item");
	ui->treeWidget->addTopLevelItem(treeItem);

	QTreeWidgetItem * treeItem2 = new QTreeWidgetItem;
	treeItem2->setText(0, "My Test Subitem");
	treeItem->addChild(treeItem2);


	QTableWidgetItem *tableItem = new QTableWidgetItem;
	tableItem->setText("Testing1");
	ui->tableWidget->setItem(0, 0, tableItem);

	QTableWidgetItem *tableItem2 = new QTableWidgetItem;
	tableItem2->setText("Testing2");
	ui->tableWidget->setItem(1, 2, tableItem2);


	QString fileName = QFileDialog::getOpenFileName(this, "Your title", QDir::currentPath(), "All files (*.*) ;; Document files (*.doc *.rtf *.txt);; PNG files (*.png)");
	qDebug() << fileName;


}

MainWindow::~MainWindow()
{

#if 0
	QMessageBox::information(this, "Alert", "Just to let you know, something happened!");
	QMessageBox::question(this, "Alert", "Just to let you know, something happened!");
	QMessageBox::warning(this, "Alert", "Just to let you know, something happened!");
	QMessageBox::critical(this, "Alert", "Just to let you know, something happened!");

	QMessageBox msgBox;
	msgBox.setWindowTitle("Serious Question");
	msgBox.setText("Just to let you know, something happened!");
	msgBox.addButton("Seriously Yes!", QMessageBox::YesRole);
	msgBox.addButton("no thanks", QMessageBox::NoRole);

	//msgBox.setIcon(QMessageBox::Question);

	QPixmap myIcon(":/images/icon.png");
	msgBox.setIconPixmap(myIcon);

	msgBox.exec();


	QMessageBox msgBox;
	msgBox.setWindowTitle("Serious Question");
	msgBox.setText("Am I an awesome guy?");
	QPushButton * yesButton = msgBox.addButton("Seriously Yes!", QMessageBox::YesRole);
	QPushButton * noButton = msgBox.addButton("Well no thanks", QMessageBox::NoRole);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.exec();

	if (msgBox.clickedButton() == (QAbstractButton *)yesButton)
	{
		QMessageBox::warning(this, "Yes", "Oh thanks! :)");
	}
	else if (msgBox.clickedButton() == (QAbstractButton *)noButton)
	{
		QMessageBox::warning(this, "No", "Oh why... :(");
	}
#endif



	delete ui;
}
