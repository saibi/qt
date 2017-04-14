#ifndef SCROLLTEXT_H
#define SCROLLTEXT_H

//
// Scrolling text widget from http://stackoverflow.com/questions/10651514/text-scrolling-marquee-in-qlabel
//
// modified by saibi

#include <QWidget>
#include <QStaticText>
#include <QTimer>

class ScrollText : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(QString separator READ separator WRITE setSeparator)

public:
	explicit ScrollText(QWidget *parent = 0);

public slots:
	QString text() const;
	void setText(QString text);

	QString separator() const;
	void setSeparator(QString separator);

	void setInterval(unsigned int msec) { timer.setInterval(msec); }

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void resizeEvent(QResizeEvent *);

private:
	void updateText();
	QString _text;
	QString _separator;
	QStaticText staticText;
	int singleTextWidth;
	QSize wholeTextSize;
	int leftMargin;
	bool scrollEnabled;
	int scrollPos;
	QImage alphaChannel;
	QImage buffer;
	QTimer timer;

private slots:
	virtual void timer_timeout();
};

#endif // SCROLLTEXT_H
