#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:34:01
#
#-------------------------------------------------

QT     += widgets
CONFIG += c++11

TARGET = gui
TEMPLATE = lib

DEFINES += GUI_LIBRARY

SOURCES +=\
    tooling.cpp \
    MainWindow.cpp \
    ObservationPane.cpp \
    SnapshotPane.cpp \
    SweepPane.cpp \
    CameraButtonBox.cpp \
    DatagramBox.cpp \
    AbstractMainPane.cpp \
    ConfigurationDlg.cpp

HEADERS +=\
    gui_global.h \
    tooling.h \
    MainWindow.h \
    ObservationPane.h \
    SnapshotPane.h \
    SweepingPane.h \
    CameraButtonBox.h \
    DatagramBox.h \
    AbstractMainPane.h \
    ConfigurationDlg.h

RESOURCES += \
    resources.qrc

unix {
    target.path = /usr/lib
    INSTALLS += target
}
