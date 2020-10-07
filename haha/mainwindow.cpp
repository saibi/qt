#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("all files (*.*)"));
	if ( !fileName.isEmpty() ) {
		statusBar()->showMessage(tr("File Loaded"), 2000);
	}
}

void MainWindow::on_actionAbout_CDRMate_triggered()
{
	QMessageBox::about(this, tr("About CDRMate"),
		tr(	"<H2>CDRMate</H2>"
			"<p>Version 2.0 (Build xxxxx)</p>"
			));
}
