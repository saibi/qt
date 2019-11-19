#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>
#include <QSqlQuery>

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QBarSet *set0 = new QBarSet("Jane");
	QBarSet *set1 = new QBarSet("John");
	QBarSet *set2 = new QBarSet("Axel");
	QBarSet *set3 = new QBarSet("Mary");
	QBarSet *set4 = new QBarSet("Samantha");

	*set0 << 10 << 20 << 30 << 40 << 50 << 60;
	*set1 << 50 << 70 << 40 << 45 << 80 << 70;
	*set2 << 30 << 50 << 80 << 13 << 80 << 50;
	*set3 << 50 << 60 << 70 << 30 << 40 << 25;
	*set4 << 90 << 70 << 50 << 30 << 16 << 42;

	QBarSeries *seriesBar = new QBarSeries();
	seriesBar->append(set0);
	seriesBar->append(set1);
	seriesBar->append(set2);
	seriesBar->append(set3);
	seriesBar->append(set4);


	QChart *chartBar = new QChart();
	chartBar->addSeries(seriesBar);
	chartBar->setTitle("Students Performance");
	chartBar->setAnimationOptions(QChart::SeriesAnimations);

	QStringList categories;
	categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	chartBar->createDefaultAxes();
	chartBar->setAxisX(axis, seriesBar);

	chartViewBar = new QChartView(chartBar);
	chartViewBar->setRenderHint(QPainter::Antialiasing);
	chartViewBar->setParent(ui->chart1);


	QPieSeries *seriesPie = new QPieSeries();
	seriesPie->append("Jane", 10);
	seriesPie->append("Joe", 20);
	seriesPie->append("Andy", 30);
	seriesPie->append("Barbara", 40);
	seriesPie->append("Jason", 50);

	QPieSlice *slice = seriesPie->slices().at(1);
	slice->setExploded();
	slice->setLabelVisible();
	slice->setPen(QPen(Qt::darkGreen, 2));
	slice->setBrush(Qt::green);

	QChart *chartPie = new QChart();
	chartPie->addSeries(seriesPie);
	chartPie->setTitle("students Performance");

	chartViewPie = new QChartView(chartPie);
	chartViewPie->setRenderHint(QPainter::Antialiasing);
	chartViewPie->setParent(ui->chart2);


	QLineSeries *seriesLine = new QLineSeries();
	seriesLine->append(0, 6);
	seriesLine->append(2, 4);
	seriesLine->append(3, 8);
	seriesLine->append(7, 4);
	seriesLine->append(10, 5);
	seriesLine->append(11, 10);
	seriesLine->append(13, 3);
	seriesLine->append(17, 6);
	seriesLine->append(18, 3);
	seriesLine->append(20, 2);

	QChart *chartLine = new QChart();
	chartLine->addSeries(seriesLine);
	chartLine->createDefaultAxes();
	chartLine->setTitle("Students Performance");

	chartViewLine = new QChartView(chartLine);
	chartViewLine->setRenderHint(QPainter::Antialiasing);
	chartViewLine->setParent(ui->chart3);






	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

	db.setHostName("127.0.0.1");
	db.setPort(3306);
	db.setDatabaseName("saibi_db");
	db.setUserName("test");
	db.setPassword("test");

	if ( db.open() )
	{
		qDebug() << "Connected!";
	}
	else
	{
		qDebug() << "Failed to connect";
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_loginButton_clicked()
{

	QString username = ui->userInput->text();
	QString password = ui->passwordInput->text();

	qDebug() << username << password;

	QString command = "SELECT * FROM user WHERE username = '" + username + "' AND password = '" + password + "' AND status = 0";

	QSqlQuery query(db);

	if ( query.exec(command) )
	{
		if ( query.size() > 0 )
		{
			QMessageBox::information(this, "Login success.", "You successfully logged in!");

		}
		else
		{
			QMessageBox::information(this, "Login failed.", "Login failed. Please try again...");
		}
	}
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
	QMainWindow::resizeEvent(event);

	chartViewBar->resize(chartViewBar->parentWidget()->size());
	chartViewPie->resize(chartViewPie->parentWidget()->size());
	chartViewLine->resize(chartViewLine->parentWidget()->size());
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
	if ( arg1 == 1 )
	{
		// do sql query
	}
}
