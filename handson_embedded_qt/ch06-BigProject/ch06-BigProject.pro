QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quick quickwidgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mocktempsensor.cpp \
    temperaturehistoryform.cpp \
    temperaturesensorif.cpp

HEADERS += \
    mainwindow.h \
    mocktempsensor.h \
    temperaturehistoryform.h \
    temperaturesensorif.h

FORMS += \
    mainwindow.ui \
    temperaturehistoryform.ui

# Default rules for deployment.
linux-rk3399-g++: target.path = /tmp
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	qml.qrc
