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
    Core.cpp \
    Crystal.cpp \
    ImageBuffer.cpp \
    controllers.cpp \
    mockups.cpp \
    drivers.cpp

HEADERS += \
    core_global.h \
    Core.h \
    Crystal.h \
    ImageBuffer.h \
    controllers.h \
    mockups.h \
    drivers.h \
    Snapshot.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
