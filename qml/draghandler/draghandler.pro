QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =




linux-rk3399mali-g++ | linux-rk3399x11-g++ {
    DEFINES += __RK3399__

    linux-rk3399mali-g++ {
        DEFINES += __RK3399_MALI__
    } else {
        DEFINES += __RK3399_X11__
    }

    target.path = /home/saibi/cam_gst_qt
    INSTALLS += target

    PKG_CONFIG_PATH=
    PKG_CONFIG_LIBDIR=$$[QT_SYSROOT]/usr/lib/aarch64-linux-gnu/pkgconfig:$$[QT_SYSROOT]/usr/lib/pkgconfig:$$[QT_SYSROOT]/usr/share/pkgconfig
    PKG_CONFIG_SYSROOT_DIR=$$[QT_SYSROOT]
}

equals(QT_MAJOR_VERSION, 5):equals(QT_MINOR_VERSION, 9):linux-g++ {
    message("Qt 5.9, ymkim 1810tz laptop")
    DEFINES += __1810TZ__
}

equals(QT_MAJOR_VERSION, 5): lessThan(QT_MINOR_VERSION, 12) {
    message("Use 5.12.x or later")
}
