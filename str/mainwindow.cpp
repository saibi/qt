#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QString org = "section@key";

	QString ptn = "section@";

	QString key = org.section(ptn, 1,1);

	qDebug("DBG key = [%s]", qPrintable(key));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	QString verStr = ui->lineEdit_motorVer->text().section("_", 0, 0);

	int major = verStr.section(".", 0, 0).toInt();
	int minor = verStr.section(".", 1, 1).toInt();
	QString patchStr = verStr.section(".", 2, 2);

	int patch = patchStr.section(QRegExp("\\D"), 0, 0).toInt();

	QMessageBox::information(this, "test", QString("major %1\nminor %2\npatch %3").arg(major).arg(minor).arg(patch), QMessageBox::Ok);


}

void MainWindow::on_pushButton_wps_clicked()
{
	QString wpsStr = ui->lineEdit_wps->text();

	qDebug("wpsStr.contains = %d", wpsStr.contains("wps hole") ? 1 : 0);

	const QString &arg1 = wpsStr.section(' ', 1, 1, QString::SectionSkipEmpty);

	qDebug("%s", qPrintable(arg1));
}

void MainWindow::on_pushButton_2_clicked()
{
	qsrand( QDateTime::currentDateTime().toMSecsSinceEpoch() );

	int ch = ((qrand() % 95) + '!');
	qDebug("%d , %c", ch, ch );

}
