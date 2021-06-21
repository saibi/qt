QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quick quickwidgets

QT += sql

QT += scxml

QT += virtualkeyboard

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hvaccontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    mocktempsensor.cpp \
    temperaturehistoryform.cpp \
    temperaturereading.cpp \
    temperaturesensorif.cpp \
    temperaturestorage.cpp

HEADERS += \
    hvaccontroller.h \
    mainwindow.h \
    mocktempsensor.h \
    temperaturehistoryform.h \
    temperaturereading.h \
    temperaturesensorif.h \
    temperaturestorage.h

FORMS += \
    mainwindow.ui \
    temperaturehistoryform.ui

# Default rules for deployment.
linux-rk3399-g++: target.path = /tmp
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	qml.qrc

STATECHARTS += \
	HVACControl.scxml
