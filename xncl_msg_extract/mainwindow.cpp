#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QInputDialog>

#include "xnclmessageextractor.h"
#include "version.h"
#include "extractor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->setWindowTitle( TITLE_STR " " VERSION_STR);

	m_context = "XML_Menu";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_file_open_clicked()
{
	qDebug("[UI] MainWindow::on_btn_file_open_clicked");

	// open
	QString dirPath = "";
	QString selectedFilter("");
	static const QString filter ("xml (*.xml)");

	m_file_list.clear();
	m_file_list = QFileDialog::getOpenFileNames(this, tr("Open"), dirPath, filter, &selectedFilter);
	if (m_file_list.count() == 0)
		return;

	ui->list_files->clear();	
	ui->list_files->addItems(m_file_list);
}


void MainWindow::on_btn_extract_clicked()
{
	qDebug("[UI] MainWindow::on_btn_extract_clicked");

	QString outFile = "xml_menu_message.h";
	bool result = false;

	if ( ui->radioButton_section->isChecked())
	{
		result = Extractor::extract(m_file_list, outFile, ui->checkBox->isCheckable(), Extractor::TYPE_SECTION);
	}
	else if ( ui->radioButton_file->isChecked() )
	{
		result = Extractor::extract(m_file_list, outFile, ui->checkBox->isCheckable(), Extractor::TYPE_FILENAME);
	}
	else
	{
		result = Extractor::extract(m_file_list, outFile, ui->checkBox->isCheckable(), Extractor::TYPE_STR, m_context);
	}

	if ( result )
	{
		for(int i = 0 ; i < ui->list_files->count() ; i++)
		{
			ui->list_files->item(i)->setCheckState(Qt::Checked);
		}
		QMessageBox::information(this, "Extract", outFile + " created.");
	}
	else
	{
		// error
	}

}

void MainWindow::on_btn_exit_clicked()
{
	qDebug("[UI] MainWindow::on_btn_exit_clicked");
	close();
}

void MainWindow::on_btn_open_work_directory_clicked()
{
	qDebug("[UI] MainWindow::on_btn_open_work_directory_clicked");
	system("explorer .");
}

void MainWindow::on_btn_change_context_clicked()
{
	qDebug("[UI] MainWindow::on_btn_change_context_clicked");
	QString str = QInputDialog::getText(this, "Change Context", "Context", QLineEdit::Normal, m_context);

	if ( !str.isEmpty() )
		m_context = str;

	qDebug("Context = %s", qPrintable(m_context));
}

void MainWindow::updateContextBtn()
{
	if ( ui->radioButton_fixed->isChecked() )
		ui->btn_change_context->setEnabled(true);
	else
		ui->btn_change_context->setEnabled(false);
}

void MainWindow::on_radioButton_fixed_clicked()
{
	qDebug("[UI] MainWindow::on_radioButton_fixed_clicked");
	updateContextBtn();
}

void MainWindow::on_radioButton_file_clicked()
{
	qDebug("[UI] MainWindow::on_radioButton_file_clicked");
	updateContextBtn();
}

void MainWindow::on_radioButton_section_clicked()
{
	qDebug("[UI] MainWindow::on_radioButton_section_clicked");
	updateContextBtn();
}
