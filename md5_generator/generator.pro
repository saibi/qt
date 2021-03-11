#-------------------------------------------------
#
# Project created by QtCreator 2011-09-29T09:56:33
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

TARGET = generator 
TEMPLATE = app

DESTDIR = $$PWD/bin/

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
