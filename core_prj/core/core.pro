#-------------------------------------------------
#
# Project created by QtCreator 2016-06-13T14:06:47
#
#-------------------------------------------------

QT     += gui core
CONFIG += c++11

TARGET = core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

OBJECTS_DIR=../../obj
DESTDIR=../../bin

RESOURCES += \
    scene.qrc

SOURCES += \
    Crystal.cpp \
    ImageBuffer.cpp \
    Manager.cpp \
    BaseDrivers.cpp \
    Mockups.cpp \
    Modes.cpp \
    Controllers.cpp \
    Interpolation.cpp \
    Snapshot.cpp

HEADERS += \
    core_global.h \
    Crystal.h \
    ImageBuffer.h \
    Snapshot.h \
    Manager.h \
    BaseDrivers.h \
    Controllers.h \
    Mockups.h \
    Modes.h \
    Interpolation.h

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
