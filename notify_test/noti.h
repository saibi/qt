#ifndef NOTI_H
#define NOTI_H

#include <QLabel>

class noti
{

private:
	noti();
	~noti();

public:
	static noti & instance()
	{
		static noti notification;
		return notification;
	}

	//
	void notify(const QString& message, int notifySecond = 3, bool refreshNow = false);

private:
	QLabel *m_label;

};

#endif // NOTI_H
