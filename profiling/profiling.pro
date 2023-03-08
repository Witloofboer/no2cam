#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T15:42:46
#
#-------------------------------------------------

QT     += gui core widgets
CONFIG += c++11 console

TARGET = profiling
TEMPLATE = app

OBJECTS_DIR=../obj
DESTDIR=../bin

SOURCES += \
    profiling.cpp \
    InstrumentedManager.cpp

HEADERS  += \
    InstrumentedManager.h

FORMS    +=

RESOURCES += \
    $$PWD/../gui/resources.qrc

LIBS += -L$$OUT_PWD/../bin/ -lcore

INCLUDEPATH += $$PWD/../core_prj
DEPENDPATH += $$PWD/../core_prj

LIBS += -L$$OUT_PWD/../gui/release/ -lgui

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
