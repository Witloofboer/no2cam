#-------------------------------------------------
#
# Project created by QtCreator 2016-06-13T14:06:47
#
#-------------------------------------------------

QT     -= gui
CONFIG += c++11

TARGET = core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

OBJECTS_DIR=../../obj
DESTDIR=../../bin

SOURCES += \
    Core.cpp \
    Crystal.cpp \
    AbstractCamera.cpp \
    AbstractGenerator.cpp \
    AbstractDriver.cpp

HEADERS += \
    core_global.h \
    Core.h \
    Crystal.h \
    AbstractCamera.h \
    AbstractGenerator.h \
    AbstractDriver.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
