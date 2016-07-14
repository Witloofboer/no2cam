#-------------------------------------------------
#
# Project created by QtCreator 2016-06-13T14:06:47
#
#-------------------------------------------------

QT     -= gui
CONFIG += c++11

TARGET = core
TEMPLATE = lib
CONFIG += staticlib

DEFINES += CORE_LIBRARY

SOURCES += \
    AotfCrystal.cpp \
    Core.cpp

HEADERS += \
    AotfCrystal.h \
    Core.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
