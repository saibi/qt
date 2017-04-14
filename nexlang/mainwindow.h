#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
protected:
	virtual void changeEvent(QEvent *event);

	void setLanguage(int lang);

private slots:
	void on_comboBox_currentIndexChanged(int index);

	void on_pushButton_2_clicked();

private:
	Ui::MainWindow *ui;

	QTranslator *m_translator;
};

#endif // MAINWINDOW_H
