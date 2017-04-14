#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T15:34:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = find_file_dialog
TEMPLATE = app


SOURCES += main.cpp\
        dlg_findfiles.cpp \
    util/sdobserver.cpp \
    hw/beep.cpp \
    hw/beepthread.cpp \
    ui/keyboarddlg.cpp

HEADERS  += dlg_findfiles.h \
    util/sdobserver.h \
    hw/beep.h \
    hw/beepthread.h \
    ui/keyboarddlg.h

FORMS    += dlg_findfiles.ui \
    ui/keyboarddlg.ui
