QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = test

DEPENDPATH += .
INCLUDEPATH += .

linux-arm-a20-g++ { 
	QMAKE_CXXFLAGS += -D__arm_A20__
	QMAKE_CXXFLAGS += -mfpu=neon -ftree-vectorize -flax-vector-conversions
}

HEADERS += \
	mythread.h \
	imageviewer.h \
	neon.h \
	camera.h \
	led.h

SOURCES += \
	camera.cpp \
	mythread.cpp \
	imageviewer.cpp \
	neon.cpp \
	main.cpp \
	led.cpp

FORMS += \
	imageviewer.ui

