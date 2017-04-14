#-------------------------------------------------
#
# Project created by QtCreator 2016-01-07T15:44:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = coverage
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

exists(cov) {
	message("coverage test")
	QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
	QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
	LIBS += -lgcov
}

