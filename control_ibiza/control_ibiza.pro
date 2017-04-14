#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T09:53:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = control_ibiza
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    buttoncontroller.cpp

HEADERS  += mainwindow.h \
    buttoncontroller.h

FORMS    += mainwindow.ui
