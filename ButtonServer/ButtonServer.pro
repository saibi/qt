#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T18:39:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ButtonServer
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    buttonwidget.cpp

HEADERS  += widget.h \
    buttonwidget.h

FORMS    += widget.ui \
    buttonwidget.ui
