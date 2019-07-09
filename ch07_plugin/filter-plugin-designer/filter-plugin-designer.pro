#-------------------------------------------------
#
# Project created by QtCreator 2019-05-02T15:50:28
#
#-------------------------------------------------

QT += widgets uiplugin

CONFIG += plugin
CONFIG += c++14

TEMPLATE = lib
DEFINES += FILTERPLUGINDESIGNER_LIBRARY

TARGET = $$qtLibraryTarget($$TARGET)
INSTALLS += target

windows {
target.path = $$(QTDIR)/../../Tools/QtCreator/bin/plugins/designer

debug:target_lib.files = $$OUT_PWD/debug/$${TARGET}.lib
release:target_lib.files = $$OUT_PWD/release/$${TARGET}.lib
target_lib.path = $$(QTDIR)/../../Tools/QtCreator/bin/plugins/designer

INSTALLS += target_lib

INCLUDEPATH += $$(OPENCV_HOME)/../../include
LIBS += -L$$(OPENCV_HOME)/lib -lopencv_core2413 -lopencv_imgproc2413
}


linux {
target.path = $$(QTDIR)/../../Tools/QtCreator/lib/Qt/plugins/designer/

CONFIG += link_pkgconfig
PKGCONFIG += opencv
}


macx {
target.path = "$$(QTDIR)/../../QtCreator.app/Contents/PlugIns/designer/"
target_lib.files = $$OUT_PWD/lib$${TARGET}.dylib
target_lib.path = "$$(QTDIR)/../../QtCreator.app/Contents/PlugIns/designer/"

INSTALLS += target_lib
INCLUDEPATH += /usr/local/Cellar/opencv/2.4.13/include/
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc
}


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        filterplugindesigner.cpp \
    filter.cpp \
	filteroriginal.cpp \
    filterblur.cpp \
    filtergrayscale.cpp \
    filterwidget.cpp

HEADERS += \
        filterplugindesigner.h \
        filter-plugin-designer_global.h \ 
    filter.h \
	filteroriginal.h \
    filterblur.h \
    filtergrayscale.h \
    filterwidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    filterwidget.ui

RESOURCES += \
    filter-plugin-designer.qrc
