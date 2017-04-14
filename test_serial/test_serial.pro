# -------------------------------------------------
# Project created by QtCreator 2010-08-16T11:44:58
# -------------------------------------------------
TARGET = test_serial
TEMPLATE = app
SOURCES += main.cpp \
    dialog.cpp \
    qextserialport/qextserialport.cpp \
    qextserialport/qextserialbase.cpp \
    record.cpp \
    recordset.cpp \
    packet.cpp \
    parser.cpp \
    serialport.cpp \
    serialhandler.cpp \
    serialmanager.cpp \
    serialdialog.cpp
HEADERS += dialog.h \
    qextserialport/qextserialport.h \
    qextserialport/qextserialbase.h \
    record.h \
    recordset.h \
    packet.h \
    parser.h \
    serialport.h \
    serialhandler.h \
    serialmanager.h \
    serialdialog.h
FORMS += dialog.ui \
    serialdialog.ui
unix:HEADERS += qextserialport/posix_qextserialport.h
unix:SOURCES += qextserialport/posix_qextserialport.cpp
unix:DEFINES += _TTY_POSIX_
win32:HEADERS += qextserialport/win_qextserialport.h
win32:SOURCES += qextserialport/win_qextserialport.cpp
win32:DEFINES += _TTY_WIN_
