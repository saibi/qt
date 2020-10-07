#-------------------------------------------------
#
# Project created by QtCreator 2010-03-04T09:07:51
#
#-------------------------------------------------

TARGET = qmake_test
TEMPLATE = app

macx {
        message($$[QT_VERSION])
        message($$QMAKESPEC)
        DESTDIR=$$(PWD)
        message(The project will be installed in $$DESTDIR)
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
