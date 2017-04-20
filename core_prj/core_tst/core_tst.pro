TEMPLATE = app

QT += testlib
QT -= gui

CONFIG += c++11
CONFIG += testcase
CONFIG += console
CONFIG -= app_bundle

TARGET = core_tst

INCLUDEPATH += ../..
OBJECTS_DIR = ../../obj
DESTDIR=../../bin

SOURCES += \
    core_tst.cpp \
    CrystalTest.cpp \
    InterpolateTest.cpp

HEADERS += \
    CrystalTest.h \
    InterpolateTest.h

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

LIBS += -L$$OUT_PWD/../../bin/ -lcore

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
