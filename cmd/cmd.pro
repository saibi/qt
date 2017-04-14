#-------------------------------------------------
#
# Project created by QtCreator 2012-07-27T11:55:15
#
#-------------------------------------------------

QT       += core gui

TARGET = cmd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlg_progress.cpp \
    cmdthread.cpp \
    progressthread.cpp

HEADERS  += mainwindow.h \
    dlg_progress.h \
    cmdthread.h \
    progressthread.h

FORMS    += mainwindow.ui \
    dlg_progress.ui
