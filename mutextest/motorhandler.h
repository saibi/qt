#ifndef MOTORHANDLER_H
#define MOTORHANDLER_H

#include <QObject>
#include <QMutexLocker>

class MotorHandler : public QObject
{
	Q_OBJECT
public:
	explicit MotorHandler(QObject *parent = 0);
	~MotorHandler();

	Q_DISABLE_COPY(MotorHandler)

public:
	static MotorHandler& instance()
	{
		static MotorHandler motorhandler;
		return motorhandler;
	}

	void test(const QString & msg = 0);

	void hello(const QString & msg = 0);

signals:

public slots:


private:
	QMutex m_mutex;

};

#endif // MOTORHANDLER_H
