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
    AotfCrystal.cpp \
    Core.cpp

HEADERS += \
    core_global.h \
    AotfCrystal.h \
    Core.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
