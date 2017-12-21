#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QTime>
#include <QPoint>

class ClickableLabel : public QLabel
{
	Q_OBJECT

public:
	explicit ClickableLabel(QWidget *parent = 0);

	bool isLongPressEnabled() {return m_longPressEnabled;}
	void setLongPressEnabled(bool enabled = true) {m_longPressEnabled = enabled;}
	void setLongPressMSec(int msec) { if ( msec < 500 ) msec = 500; m_longPressMSec = msec; }
	void setPressReleaseEnabled(bool enabled = true) {m_pressReleaseEnabled = enabled;}

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

	void timerEvent(QTimerEvent *e);

private:
	QTime m_lastClickTime;

	bool m_longPressEnabled;
	int m_longPressTimerId;
	bool m_longPressed;
	int m_longPressMSec;
	bool m_pressReleaseEnabled;

signals:
	void clicked(const QPoint &pos);
	void longPressed();
	void pressed(const QPoint &pos);
	void released(const QPoint &pos);

public slots:
};

#endif // CLICKABLELABEL_H
