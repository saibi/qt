#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T15:34:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nexfiledialog
TEMPLATE = app


SOURCES += main.cpp\
		nexfiledialog.cpp \
    util/sdobserver.cpp \
    hw/beep.cpp \
    hw/beepthread.cpp \
    ui/keyboarddlg.cpp

HEADERS  += nexfiledialog.h \
    util/sdobserver.h \
    hw/beep.h \
    hw/beepthread.h \
    ui/keyboarddlg.h

FORMS    += nexfiledialog.ui \
    ui/keyboarddlg.ui
