#-------------------------------------------------
#
# Project created by QtCreator 2016-01-28T14:01:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = notify_test
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    notification.cpp \
    notificationwidget.cpp \
    noti.cpp

HEADERS  += dialog.h \
    notification.h \
    notificationwidget.h \
    noti.h

FORMS    += dialog.ui \
    notificationwidget.ui
