#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T11:04:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thread
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    worker.cpp \
    waitindicator.cpp

HEADERS  += mainwindow.h \
    worker.h \
    waitindicator.h

FORMS    += mainwindow.ui
