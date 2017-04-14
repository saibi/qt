#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


	m_bevel.append(1);
	m_bevel.append(2);
	m_bevel.append(3);
	m_bevel.append(4);
	m_bevel.append(5);

	m_rimless.append(10);
	m_rimless.append(20);
	m_rimless.append(30);
	m_rimless.append(40);
	m_rimless.append(50);

	m_size = m_bevel.size();

	m_idxStart = -1;
	m_idxEnd = -1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_test_clicked()
{
	m_idxStart = ui->lineEdit_start->text().toInt();
	m_idxEnd = ui->lineEdit_end->text().toInt();

	if ( m_idxStart < 0 || m_idxStart >= m_size )
	{
		qDebug("invalid start value");
		return;
	} else if ( m_idxEnd < 0 || m_idxEnd >= m_size )
	{
		qDebug("invalid end value");
		return;
	}
	else
	{
		QList <int> result;


		if ( m_idxStart <= m_idxEnd )
			result = m_rimless.mid(0, m_idxStart ) + m_bevel.mid(m_idxStart, m_idxEnd - m_idxStart + 1) + m_rimless.mid(m_idxEnd + 1, m_size - m_idxEnd - 1 );
		else
			result = m_bevel.mid(0, m_idxEnd + 1 ) + m_rimless.mid(m_idxEnd + 1, m_idxStart - m_idxEnd - 1) + m_bevel.mid(m_idxStart, m_size - m_idxStart );


		qDebug("result size = %d", result.size());
		for (int i = 0 ; i < result.size() ; i++ )
		{
			qDebug("[%d] %d", i, result[i]);
		}
	}
}
