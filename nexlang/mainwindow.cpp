#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_translator = 0;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::setLanguage(int lang)
{
	if (m_translator != 0 )
	{
		qApp->removeTranslator(m_translator);
		delete m_translator;
		m_translator = 0;
	}

	switch (lang)
	{
	case 1: // kor
		m_translator = new QTranslator(this);
		m_translator->load(QString::fromUtf8(":/global/nexlang_ko.qm"));
		qApp->installTranslator(m_translator);
		update();
		break;

	default:
		break;
	}
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
	qDebug("language = %d", index);


}

void MainWindow::on_pushButton_2_clicked()
{
	setLanguage(ui->comboBox->currentIndex());
}
