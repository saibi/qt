#-------------------------------------------------
#
# Project created by QtCreator 2019-12-06T17:46:48
#
#-------------------------------------------------

include(../qmake-target-platform.pri)
include(../qmake-destination-path.pri)

DESTDIR = $$PWD/../binaries/$$DESTINATION_PATH
OBJECTS_DIR = $$PWD/build/$$DESTINATION_PATH/.obj
MOC_DIR = $$PWD/build/$$DESTINATION_PATH/.moc
RCC_DIR = $$PWD/build/$$DESTINATION_PATH/.qrc
UI_DIR = $$PWD/build/$$DESTINATION_PATH/.ui

QT       -= gui

TARGET = cm-lib
TEMPLATE = lib

CONFIG += c++14

DEFINES += CMLIB_LIBRARY

INCLUDEPATH += source

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
	source/models/client.cpp \
	source/controllers/master-controller.cpp \
	source/controllers/command-controller.cpp \
	source/framework/command.cpp

HEADERS += \
	source/models/client.h \
	source/cm-lib_global.h \
	source/controllers/master-controller.h \
	source/controllers/navigation-controller.h \
	source/controllers/command-controller.h \
	source/framework/command.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

message(cm-lib project dir: $${PWD})

message(cm-lib output dir: $${DESTDIR})

