#-------------------------------------------------
#
# Project created by QtCreator 2018-01-12T14:49:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

TARGET = a20_layer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


linux-arm-a20-g++ {
		QMAKE_CXXFLAGS += -D__arm_A20__
		QMAKE_CXXFLAGS += -mfpu=neon -ftree-vectorize -flax-vector-conversions
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    camthread.cpp \
    neon.cpp \
    hw/beep.cpp \
    hw/beepthread.cpp \
    keypaddlg.cpp \
    a20disp.cpp

HEADERS += \
        mainwindow.h \
    camthread.h \
    neon.h \
    hw/beep.h \
    hw/beepthread.h \
    keypaddlg.h \
    sunxi_disp_ioctl.h \
    a20disp.h

FORMS += \
        mainwindow.ui \
    keypaddlg.ui

RESOURCES += \
    resources.qrc
