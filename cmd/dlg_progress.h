#ifndef DLG_PROGRESS_H
#define DLG_PROGRESS_H

#include <QDialog>

namespace Ui {
class Dlg_Progress;
}

class Dlg_Progress : public QDialog
{
	Q_OBJECT
	
public:
	explicit Dlg_Progress(QWidget *parent = 0);
	~Dlg_Progress();
	void setTime(int seconds);
	void setPercent(float percent);
	void terminate();
	bool isStarted();

protected:
	virtual void timerEvent(QTimerEvent *);
	virtual void showEvent(QShowEvent *);
	virtual void hideEvent(QHideEvent *);
	
private:
	Ui::Dlg_Progress *ui;
	int m_timerId;
	int m_maxSec;
	bool m_started;
};

#endif // DLG_PROGRESS_H
