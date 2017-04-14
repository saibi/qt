#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T11:42:43
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = xncl_extractor_cmd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $$PWD/bin/


SOURCES += console_main.cpp \
    cmdproc.cpp \
    extractor.cpp \
    xnclmessageextractor.cpp

HEADERS += \
    cmdproc.h \
    extractor.h \
    version.h \
    xnclmessageextractor.h
