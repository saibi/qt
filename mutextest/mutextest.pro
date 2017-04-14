#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T14:53:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mutextest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    notification.cpp \
    notificationwidget.cpp \
    sdobserver.cpp \
    sdobserver2.cpp \
    sdobserver3.cpp \
    dataconverter.cpp \
    motorhandler.cpp

HEADERS  += mainwindow.h \
    notification.h \
    notificationwidget.h \
    sdobserver.h \
    sdobserver2.h \
    sdobserver3.h \
    dataconverter.h \
    motorhandler.h

FORMS    += mainwindow.ui \
    notificationwidget.ui
