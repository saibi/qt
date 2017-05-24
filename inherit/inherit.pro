#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T10:49:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = inherit
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    childdialog.cpp

HEADERS  += dialog.h \
    childdialog.h

FORMS    += dialog.ui \
    childdialog.ui

RESOURCES += \
    resource.qrc
