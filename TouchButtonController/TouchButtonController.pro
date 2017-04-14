#-------------------------------------------------
#
# Project created by QtCreator 2015-07-24T14:55:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TouchButtonController
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    controller.cpp

HEADERS  += client.h \
    controller.h

FORMS    += client.ui
