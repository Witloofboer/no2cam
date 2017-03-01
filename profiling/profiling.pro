#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:42:46
#
#-------------------------------------------------

QT     += gui
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = profiling
TEMPLATE = app

OBJECTS_DIR=../obj
DESTDIR=../bin

SOURCES += \
    profiling.cpp

HEADERS  +=

FORMS    +=

LIBS += -L$$OUT_PWD/../bin/ -lcore

INCLUDEPATH += $$PWD/../core_prj
DEPENDPATH += $$PWD/../core_prj

LIBS += -L$$OUT_PWD/../gui/release/ -lgui

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
