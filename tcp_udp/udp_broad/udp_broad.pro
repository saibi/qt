QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    discoverlistform.cpp \
    main.cpp \
    mainwindow.cpp \
    tcppacket.cpp \
    tcppacket2.cpp \
    tcppacket3.cpp \
    tcpserver.cpp \
    tcpsocketthread.cpp

HEADERS += \
    discoverlistform.h \
    mainwindow.h \
    tcppacket.h \
    tcppacket2.h \
    tcppacket3.h \
    tcpserver.h \
    tcpsocketthread.h

FORMS += \
    discoverlistform.ui \
    mainwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
