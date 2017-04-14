#-------------------------------------------------
#
# Project created by QtCreator 2015-07-24T13:33:22
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TouchButtonServer
TEMPLATE = app


SOURCES += main.cpp\
    buttondialog.cpp \
    server.cpp

HEADERS  += \
    buttondialog.h \
    server.h

FORMS    += \
    buttondialog.ui
