#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T17:18:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shape
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    shapedata.cpp \
    drillshapedata.cpp

HEADERS  += mainwindow.h \
    shapedata.h \
    drillshapedata.h

FORMS    += mainwindow.ui
