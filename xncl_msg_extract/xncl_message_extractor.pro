#-------------------------------------------------
#
# Project created by QtCreator 2011-09-29T09:56:33
#
#-------------------------------------------------

QT       += core gui xml

TARGET = xncl_extractor
TEMPLATE = app

DESTDIR = $$PWD/bin/

SOURCES += main.cpp\
        mainwindow.cpp \
    xnclmessageextractor.cpp \
    cmdproc.cpp \
    extractor.cpp

HEADERS  += mainwindow.h \
    xnclmessageextractor.h \
    cmdproc.h \
    version.h \
    extractor.h

FORMS    += mainwindow.ui
