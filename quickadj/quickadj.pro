#-------------------------------------------------
#
# Project created by QtCreator 2015-01-27T14:27:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = quickadj
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    quickadj.cpp \
    dlg_checkbox.cpp \
    dlg_step.cpp

HEADERS  += mainwindow.h \
    quickadj.h \
    dlg_checkbox.h \
    dlg_step.h

FORMS    += mainwindow.ui \
    quickadj.ui \
    dlg_checkbox.ui \
    dlg_step.ui
