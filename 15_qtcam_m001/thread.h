#include <QApplication>
#include <QMainWindow>
#include <QThread>
#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QtGui>
#include <QScrollArea>

/*
QT_BEGIN_NAMESPACE
class QApplicarionn;
class QLabel;
class QMenu;
class QScrollBar;
QT_END_NAMESPACE
*/




//#define FORCED_WIDTH    1280
//#define FORCED_HEIGHT   1024
//#define FORCED_WIDTH    960
//#define FORCED_HEIGHT   960
//#define FORCED_WIDTH    800
//#define FORCED_HEIGHT   480
#define FORCED_WIDTH    640
#define FORCED_HEIGHT   480

#define QT_WIDTH    FORCED_WIDTH
#define QT_HEIGHT   FORCED_HEIGHT


extern int getControl(int control , int *value);
extern int setControl(int control, int value);
extern int upControl(int control);
extern int downControl(int control);


extern void upControl_gain(void);
extern void downControl_gain(void);
extern void upControl_exp(void);
extern void downControl_exp(void);
extern void upControl_rowStart(void);
extern void downControl_rowStart(void);
extern void upControl_colStart(void);
extern void downControl_colStart(void);



extern void open_device(void);
extern void init_device(void);
extern void start_capturing(void);
extern int mainloop(void);
extern void stop_capturing(void);
extern void uninit_device(void);
extern void close_device(void);
extern void g_led_on(void);
extern void g_led_off(void);


struct buffer {
    void   *start;
    size_t  length;
};


class MyThread : public QThread
{

  Q_OBJECT

private:
    void run(); //Q_DECL_OVERRIDE
    // {
    //     qDebug() << __func__ << "in MyThread" << currentThread();
   //  }
signals:
	void updateThread(int count);
};



class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer();

public slots:
	void updateGUI(int count);

protected:
	void keyPressEvent(QKeyEvent *event);

};



