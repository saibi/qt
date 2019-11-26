#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::testString()
{
	QString text = "Testing";

	QVERIFY(text.toUpper() == "TESTING");
}

void MainWindow::testGui()
{
	QTest::keyClicks(ui->lineEdit, "testing gui");

	QCOMPARE(ui->lineEdit->text(), QString("testing gui"));
}
