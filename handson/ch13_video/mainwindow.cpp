#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	player = new QMediaPlayer(this);

	QVideoWidget * videoWidget = new QVideoWidget(this);
	player->setVideoOutput(videoWidget);
	ui->movieLayout->layout()->addWidget(videoWidget);

	player->setVolume(ui->horizontalSlider_vol->value());

	connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::stateChanged);
	connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select Video File", qApp->applicationDirPath(), "Video Files (*.mp4 *.mov *.mkv)");
	QFileInfo fileInfo(fileName);

	player->setMedia(QUrl::fromLocalFile(fileName));

	if ( player->isMetaDataAvailable() )
	{
		QString title = player->metaData(QMediaMetaData::AlbumTitle).toString();
		ui->label_title->setText("Playing " + title);

	}
	else
	{
		ui->label_title->setText("Playing " + fileInfo.fileName());
	}

	ui->pushButton_play->setEnabled(true);
	ui->pushButton_play->click();
}

void MainWindow::stateChanged(QMediaPlayer::State state)
{
	if ( state == QMediaPlayer::PlayingState)
	{
		ui->pushButton_play->setEnabled(false);
		ui->pushButton_pause->setEnabled(true);
		ui->pushButton_stop->setEnabled(true);
	}
	else if ( state == QMediaPlayer::PausedState )
	{
		ui->pushButton_play->setEnabled(true);
		ui->pushButton_pause->setEnabled(false);
		ui->pushButton_stop->setEnabled(true);
	}
	else if ( state == QMediaPlayer::StoppedState )
	{
		ui->pushButton_play->setEnabled(true);
		ui->pushButton_pause->setEnabled(false);
		ui->pushButton_stop->setEnabled(false);

	}
}

void MainWindow::positionChanged(qint64 position)
{
	if ( ui->horizontalSlider_pos->maximum() != player->duration() )
		ui->horizontalSlider_pos->setMaximum(player->duration());

	ui->horizontalSlider_pos->setValue(position);

	int seconds = (position/1000) % 60;
	int minutes = (position/60000) % 60;
	int hours = (position/3600000) % 24;

	QTime time(hours, minutes, seconds);
	ui->label_time->setText(time.toString());
}


void MainWindow::on_pushButton_play_clicked()
{
	player->play();
}

void MainWindow::on_pushButton_pause_clicked()
{
	player->pause();
}

void MainWindow::on_pushButton_stop_clicked()
{
	player->stop();
}

void MainWindow::on_horizontalSlider_vol_sliderMoved(int position)
{
   player->setVolume(position);
}

void MainWindow::on_horizontalSlider_pos_sliderMoved(int position)
{
	player->setPosition(position);
}
